#include "controller_utils.h"

// Map function similar to Arduino's map()
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int clamp(int value, int min_val, int max_val)
{
    if (value < min_val)
        return min_val;
    if (value > max_val)
        return max_val;
    return value;
}

// Cubic coefficients for length as a function of ADC
constexpr double A_CUB = 8.578477034210919e-9;
constexpr double B_CUB = -1.1010212801757337e-5;
constexpr double C_CUB = 5.223133340568266e-3;
constexpr double D_CUB = -5.38998286186248e-1;

// Function to compute length from ADC using cubic model
constexpr float cubic(float y) {
    return (A_CUB * y * y * y + B_CUB * y * y + C_CUB*y + D_CUB)*1000;
}

ControllerUtil::ControllerUtil(IO *io, ProfileController *profilecontroller, LinearControl *handle_controller, ADS1232 *ads, TouchController *touchController, UART *uart, Menu *menu, motor *stepper, int *button)
    : io(io),
      profilecontroller(profilecontroller),
      handle_controller(handle_controller),
      ads(ads),
      touchController(touchController),
      uart(uart),
      button(button),
      menu(menu),
      stepper(stepper)
{
}

void ControllerUtil::callibrateADS1232_weight(float known_weight)
{
    // io->controlLEDs(0b0010, true); // Turn on LED 0 to indicate calibration mode
    // menu->showCalibrationScreen(); // Show calibration screen on display
    // _delay_ms(2000);                // Wait for 0.5 seconds to indicate start of calibration

    // ads->calibrate();                // Calibrate ADS1232 to find offset
    // uart->println(ads->getOffset()); // Print offset value

    
    // // here need to get init value and known weight 


    // menu->showPlaceWeightScreen(); // Show screen to place weight
    // _delay_ms(500);                // Wait for 1 second to indicate start of calibration

    // while (*button != -4)
    // {
    //     handlLinearControl(); // Handle linear control if available
    // }
    // io->controlLEDs(0b0100, true);   // Turn off all LEDs after calibration
    // *button = 0;
    // adc = ads->getAverage(20); // Get initial ADC value
    // // turn motor by 120 deg to lift


    // // while (*button != -4)
    // // {
    // //     io->controlLEDs(0b0001, true); // Blink LED 1 to indicate waiting for button press
    // //     _delay_ms(250);                // Wait for 0.5 seconds
    // //     io->controlLEDs(0b0000, true); // Turn off all LEDs
    // //     _delay_ms(250);                // Wait for 0.5 seconds
    // //     current_adc = ads->getAverage(10); // Get current ADC value
    // //     if (abs(current_adc - prev_adc) < 500){ 
    // //         handleFloatControl(); // Handle float control if ADC value is stable
    // //         menu->showPressButtonScreen(); // Show screen to press button
    // //     } else {
    // //         profilecontroller->run(30); // Stop motor if ADC value is not stable
    // //     }

    // // }


    // ads->CalcScale(5000.0f);   // Scale calibration with known weight (e.g., 2500g)

    // menu->showDoneCalibrationScreen(); // Show calibration screen on display
    // io->controlLEDs(0b0100, true); // Turn off all LEDs after calibration
    // _delay_ms(500);                // Wait for 1 second to indicate end of calibration
    // io->controlLEDs(0b0000, true); // Turn off all LEDs
}

void ControllerUtil::handlLinearControl()
{
    linear_value = handle_controller->get_filtered(); // Get filtered value from LinearControl

    // sprintf(buffer, "linear value: %.2f\n", linear_value); // Format output
    // uart->transmitString(buffer); // Send filtered value over UART

    // speed = pow((linear_value - 10) / 830, 0.5) * 1000;
    // x = x/1024*1000;  // Scale the filtered value

    if((linear_value  < 820.0f) && (linear_value > 780.0f)) {
        speed = 0; // Set speed to 0 if value is out of range
    } else {
        speed = int(cubic(linear_value)); // Convert filtered value to speed using cubic model
        speed = map(speed, 0, 2000, -100, 100); // Map the speed value to a range
        
        if (abs(speed - prv_speed) < 5){                   
            speed = prv_speed; // Use previous speed if change is small
        }

    }




    prv_speed = speed; // Update previous speed
    // stepper->speedcontrol(speed);  // Control motor speed based on filtered value
    profilecontroller->run(speed); // Use profile controller to set speed

    // sprintf(buffer, "out rpm: %d, %.2f\n", speed, linear_value); // Format output string
    // uart->transmitString(buffer);                                // Send filtered value over UART
}

void ControllerUtil::handleButtonControl()
{
    if (*button == 2)
    {
        // stepper->speedcontrol(30);     // Set motor speed to 30 RPM
        profilecontroller->run(30);   // Use profile controller to set speed
        io->controlLEDs(0b1000, true); // Turn on LED 0
    }
    else if (*button == 3)
    {
        // stepper->speedcontrol(-30);    // Set motor speed to -30 RPM
        profilecontroller->run(-30);  // Use profile controller to set speed
        io->controlLEDs(0b0100, true); // Turn on LED 1
    }
    else
    {
        // stepper->speedcontrol(0);      // Stop motor if no button pressed
        profilecontroller->run(0);     // Use profile controller to stop motor
        io->controlLEDs(0b0000, true); // Turn off all LEDs
    }
}

void ControllerUtil::handleADS1232Control()
{

    // if (every_5_seconds()){
    //     touchController->updateInitial(ads->getAverage(50)); // Update initial touch value
    // }

    touchController->updateSpeed(ads->getAverage(10)); // Update speed based on ADS1232 filtered value
    // stepper->speedcontrol(touchController->getSpeed()); // Set motor speed based on touch controller
    profilecontroller->run(touchController->getSpeed()); // Use profile controller to set speed

    // uart->println(touchController->getInitial()); // Print initial value
    // uart->println(touchController->getError());   // Print speed value
}

void ControllerUtil::handleFloatControl()
{
    profilecontroller->run(0); // Use profile controller to stop motor
}


void ControllerUtil::initCalibration(){
    io->controlLEDs(0b0010, true); // Turn on LED 0 to indicate calibration mode
    menu->showCalibrationScreen(); // Show calibration screen on display
    _delay_ms(2000);                // Wait for 0.5 seconds to indicate start of calibration

    ads->calibrate();                // Calibrate ADS1232 to find offset
    uart->println(ads->getOffset()); // Print offset value

    
    // here need to get init value and known weight 


    menu->showPlaceWeightScreen(); // Show screen to place weight
    _delay_ms(500);                // Wait for 1 second to indicate start of calibration

}

void ControllerUtil::zeroGravity(){
    while (*button != -4)
    {
        handlLinearControl(); // Handle linear control if available
        // uart->println(*button); // Print message to UART
               // Wait for 0.5 seconds
    }
    profilecontroller->run(0); // Stop motor if no button pressed
    uart->println("Calibration started"); // Print message to UART
    io->controlLEDs(0b0100, true);   // Turn off all LEDs after calibration
    *button = 0;
    menu->showPleaseWaitScreen(); // Show screen to wait for calibration
    adc = ads->getAverage(20); // Get initial ADC value
    stepper->turnAngle(120,20); // Turn motor by 120 degrees to lift


    ads->CalcScale(5000.0f);   // Scale calibration with known weight (e.g., 2500g)
    weight = ads->Weight();
    uart->println(weight,3); // Print weight value to UART
    if (weight > 7000.0f){
        menu->showWarningScreen();
        stepper->stopMotor();
        return; // Stop motor if weight exceeds 7000 grams
    } 
    // else if(weight > 5000.f){
    //     menu->showWarningScreen(); 
    //     stepper->stopMotor(); // Stop motor if weight exceeds 5000 grams
    // }
    menu->showDoneCalibrationScreen(); // Show calibration screen on display
    io->controlLEDs(0b0100, true); // Turn off all LEDs after calibration
    _delay_ms(500);                // Wait for 1 second to indicate end of calibration
    io->controlLEDs(0b0000, true); // Turn off all LEDs

    while (*button != -2){
        handleADS1232Control(); // Handle ADS1232 control
    }
    menu->set_display_power_off(); // Turn off display and LEDs
}



