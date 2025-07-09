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

ControllerUtil::ControllerUtil(IO *io, ProfileController *profilecontroller, LinearControl *handle_controller, ADS1232 *ads, TouchController *touchController, UART *uart, Menu *menu, int *button)
    : io(io),
      profilecontroller(profilecontroller),
      handle_controller(handle_controller),
      ads(ads),
      touchController(touchController),
      uart(uart),
      button(button),
      menu(menu)
{
}

void ControllerUtil::callibrateADS1232_weight(float known_weight)
{
    io->controlLEDs(0b0010, true); // Turn on LED 0 to indicate calibration mode
    menu->showCalibrationScreen(); // Show calibration screen on display
    _delay_ms(100);                // Wait for 6 seconds to allow user to remove weight

    while (*button != -4)
    {
        io->controlLEDs(0b0001, true); // Blink LED 0 to indicate waiting for button press
        touchController->updateSpeed(ads->getFiltered()); // Update speed based on ADS1232 filtered value
        _delay_ms(250);                // Wait for 0.5 seconds
        io->controlLEDs(0b0000, true); // Turn off all LEDs
        _delay_ms(250);                // Wait for 0.5 seconds
    }
    io->controlLEDs(0b0100, true);   // Turn off all LEDs after calibration
    ads->calibrate();                // Calibrate ADS1232 to find offset
    uart->println(ads->getOffset()); // Print offset value
    uart->println("Now place a known weight (e.g., 2500g) on the scale.\n");
    menu->showPlaceWeightScreen(); // Show screen to place weight
    *button = 0;
    prev_adc = ads->getAverage(10); // Get initial ADC value
    while (*button != -4)
    {
        io->controlLEDs(0b0001, true); // Blink LED 1 to indicate waiting for button press
        _delay_ms(250);                // Wait for 0.5 seconds
        io->controlLEDs(0b0000, true); // Turn off all LEDs
        _delay_ms(250);                // Wait for 0.5 seconds
        current_adc = ads->getAverage(10); // Get current ADC value
        if (abs(current_adc - prev_adc) < 500){ 
            handleFloatControl(); // Handle float control if ADC value is stable
            menu->showPressButtonScreen(); // Show screen to press button
        } else {
            profilecontroller->run(30); // Stop motor if ADC value is not stable
        }

    }
    ads->CalcScale(known_weight);   // Scale calibration with known weight (e.g., 2500g)
    uart->println(ads->getScale()); // Print scale value

    menu->showDoneCalibrationScreen(); // Show calibration screen on display
    io->controlLEDs(0b0100, true); // Turn off all LEDs after calibration
    _delay_ms(500);                // Wait for 1 second to indicate end of calibration
    io->controlLEDs(0b0000, true); // Turn off all LEDs
}

void ControllerUtil::handlLinearControl()
{
    linear_value = handle_controller->get_filtered(); // Get filtered value from LinearControl
    uart->transmitString(buffer);                     // Send filtered value over UART

    speed = pow((linear_value - 10) / 830, 0.5) * 1000;
    // x = x/1024*1000;  // Scale the filtered value

    speed = map(speed, 0, 1000, -100, 100); // Map the speed value to a range

    if (abs(speed - prv_speed) < 5)
    {                      // Check if speed change is significant
        speed = prv_speed; // Use previous speed if change is small
    }

    prv_speed = speed; // Update previous speed
    // stepper->speedcontrol(speed);  // Control motor speed based on filtered value
    profilecontroller->run(speed); // Use profile controller to set speed

    sprintf(buffer, "out rpm: %d, %.2f\n", speed, linear_value); // Format output string
    uart->transmitString(buffer);                                // Send filtered value over UART
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

    uart->println(touchController->getInitial()); // Print initial value
    uart->println(touchController->getError());   // Print speed value
}

void ControllerUtil::handleFloatControl()
{
    // This function is a placeholder for future implementation of float control logic.
    // Currently, it does not perform any actions.
    // stepper->speedcontrol(0); // Stop motor if no button pressed
    profilecontroller->run(0); // Use profile controller to stop motor
}
