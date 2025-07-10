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


void ControllerUtil::handlLinearControl()
{
    linear_value = handle_controller->get_filtered(); // Get filtered value from LinearControl

    if((linear_value  < 820.0f) && (linear_value > 780.0f)) {
        speed = 0; // Set speed to 0 if value is out of range
    } else {
        speed = int(cubic(linear_value)); // Convert filtered value to speed using cubic model
        speed = map(speed, 0, 2000, -100, 100); // Map the speed value to a range
        
        if (abs(speed - prv_speed) < 5){                   
            speed = prv_speed; // Use previous speed if change is small
        }

    }

    profilecontroller->run(speed); // Use profile controller to set speed
}

void ControllerUtil::handleButtonControl()
{
    if (*button == 2)
    {
        profilecontroller->run(30);   // Use profile controller to set speed
        io->controlLEDs(0b1000, true); // Turn on LED 0
    }
    else if (*button == 3)
    {
        profilecontroller->run(-30);  // Use profile controller to set speed
        io->controlLEDs(0b0100, true); // Turn on LED 1
    }
    else
    {
        profilecontroller->run(0);     // Use profile controller to stop motor
        io->controlLEDs(0b0000, true); // Turn off all LEDs
    }
}

void ControllerUtil::handleADS1232Control()
{

    touchController->updateSpeed(ads->getAverage(10)); // Update speed based on ADS1232 filtered value
    profilecontroller->run(touchController->getSpeed()); // Use profile controller to set speed

}

void ControllerUtil::handleFloatControl()
{
    // This function is a placeholder for future implementation of float control logic.
    profilecontroller->run(0); // Use profile controller to stop motor
}


void ControllerUtil::initCalibration(){
    io->controlLEDs(0b0010, true); // Turn on LED 0 to indicate calibration mode
    menu->showCalibrationScreen(); // Show calibration screen on display
    _delay_ms(2000);                // Wait for 0.5 seconds to indicate start of calibration

    ads->calibrate();                // Calibrate ADS1232 to find offset
    uart->println(ads->getOffset()); // Print offset value


    menu->showPlaceWeightScreen(); // Show screen to place weight
    _delay_ms(500);                // Wait for 1 second to indicate start of calibration

}

void ControllerUtil::zeroGravity(){
    while (*button != -4)
    {
        handlLinearControl(); // Handle linear control if available

    }
    profilecontroller->run(0); // Stop motor if no button pressed
    uart->println("Calibration started"); // Print message to UART
    io->controlLEDs(0b0100, true);   // Turn off all LEDs after calibration
    *button = 0;
    menu->showPleaseWaitScreen(); // Show screen to wait for calibration
    adc = ads->getAverage(20); // Get initial ADC value
    stepper->turnAngle(120,30); // Turn motor by 120 degrees to lift


    ads->CalcScale(5000.0f);   // Scale calibration with known weight (e.g., 2500g)
    weight = ads->Weight();

    menu->showDoneCalibrationScreen(); // Show calibration screen on display
    io->controlLEDs(0b0100, true); // Turn off all LEDs after calibration
    _delay_ms(500);                // Wait for 1 second to indicate end of calibration
    io->controlLEDs(0b0000, true); // Turn off all LEDs
    while (*button != -2){
        handleADS1232Control(); // Handle ADS1232 control
    }
}



