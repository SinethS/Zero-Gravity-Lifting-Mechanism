#include "controller_utils.h"

// Map function similar to Arduino's map()
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int clamp(int value, int min_val, int max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}


ControllerUtil::ControllerUtil(IO *io, motor *stepper, LinearControl *handle_controller, ADS1232 *ads, TouchController* touchController, UART *uart, int *button)
    : io(io),
    stepper(stepper),
    handle_controller(handle_controller),
    ads(ads),
    touchController(touchController),
    uart(uart),
    button(button) 
{}


void ControllerUtil::callibrateADS1232_weight(float known_weight) {
    io->controlLEDs(0b0010, true); // Turn on LED 0 to indicate calibration mode
    _delay_ms(100); // Wait for 6 seconds to allow user to remove weight

    while(*button != -1){
        io->controlLEDs(0b0001, true); // Blink LED 0 to indicate waiting for button press
        _delay_ms(250); // Wait for 0.5 seconds
        io->controlLEDs(0b0000, true); // Turn off all LEDs
        _delay_ms(250); // Wait for 0.5 seconds
    }
    io->controlLEDs(0b0100, true); // Turn off all LEDs after calibration
    ads->calibrate();               // Calibrate ADS1232 to find offset
    uart->println(ads->getOffset()); // Print offset value
    uart->println("Now place a known weight (e.g., 2500g) on the scale.\n");
    *button = 0;

    while(*button != -1){
        io->controlLEDs(0b0001, true); // Blink LED 1 to indicate waiting for button press
        _delay_ms(250); // Wait for 0.5 seconds
        io->controlLEDs(0b0000, true); // Turn off all LEDs
        _delay_ms(250); // Wait for 0.5 seconds
    }
    ads->CalcScale(known_weight);        // Scale calibration with known weight (e.g., 2500g)
    uart->println(ads->getScale()); // Print scale value

    io->controlLEDs(0b0100, true); // Turn off all LEDs after calibration
    _delay_ms(500); // Wait for 1 second to indicate end of calibration
    io->controlLEDs(0b0000, true); // Turn off all LEDs
}

void ControllerUtil::handlLinearControl() {
    linear_value = handle_controller->get_filtered(); // Get filtered value from LinearControl
    uart->transmitString(buffer);  // Send filtered value over UART

    speed = pow((linear_value-10)/830, 0.5)*1000;
    // x = x/1024*1000;  // Scale the filtered value

    speed = map(speed, 0, 1000, -100, 100);  // Map the speed value to a range

    if(abs(speed - prv_speed) < 5) {  // Check if speed change is significant
        speed = prv_speed;  // Use previous speed if change is small
    }

    prv_speed = speed;  // Update previous speed
    stepper->speedcontrol(speed);  // Control motor speed based on filtered value
    // sprintf(buffer, "out rpm: %d, %.2f\n", speed, linear_value);  // Format output string
    // uart->transmitString(buffer);  // Send filtered value over UART
}

void ControllerUtil::handleButtonControl(){
     if(*button == 3){
        stepper->speedcontrol(30); // Set motor speed to 30 RPM
        io->controlLEDs(0b1000, true); // Turn on LED 0
    }
    else if(*button == 2){
        stepper->speedcontrol(-30); // Set motor speed to -30 RPM
        io->controlLEDs(0b0100, true); // Turn on LED 1
    }else{
        stepper->speedcontrol(0); // Stop motor if no button pressed
        io->controlLEDs(0b0000, true); // Turn off all LEDs
    }
}

void ControllerUtil::handleADS1232Control() {


    // if (every_5_seconds()){
    //     touchController->updateInitial(ads->getAverage(50)); // Update initial touch value
    // }

    touchController->updateSpeed(ads->getFilered()); // Update speed based on ADS1232 filtered value
    stepper->speedcontrol(touchController->getSpeed()); // Set motor speed based on touch controller

    trapspeedcontrol(-200, 0.01); // Trapezoidal speed control for the motor

    uart->println(touchController->getSpeed()); // Send message over UART

}



// Motor class speed control with trapezoidal motion profile
void ControllerUtil::trapspeedcontrol(float speed, float dt) {
    // Motion profile parameters (tune based on motor and crane)
    const float MAX_ACCEL = 250.0f; // Max acceleration (RPM/s)
    const float MAX_DECEL = -250.0f; // Max deceleration (RPM/s)
    const int MAX_RPM = 500;       // Max motor RPM
    const int MIN_RPM = -500;      // Min motor RPM

    // Static variable to track current motor speed
    int current_rpm = stepper->getCurrentRpm(); // Get current RPM from motor
    // Convert input speed to target RPM
    float target_rpm = speed * SPEED_TO_RPM;
    int direction = 1; // Default direction (1 for forward, -1 for backward)

    // Validate target RPM
    target_rpm = clamp((int)target_rpm, MIN_RPM, MAX_RPM);

    float max_change = 0.0f; // Variable to track max change in RPM

    // Apply trapezoidal profile: adjust current_rpm toward target_rpm
    if (current_rpm < target_rpm) {
        max_change = MAX_ACCEL * dt; // Use acceleration rate when speeding up
        current_rpm += max_change;
        if (current_rpm > target_rpm) {
            current_rpm = target_rpm; // Don't overshoot
        }
    } else if (current_rpm > target_rpm) {
        max_change = MAX_DECEL * dt; // Use deceleration rate when slowing down
        current_rpm += max_change;
        if (current_rpm < target_rpm) {
            current_rpm = target_rpm; // Don't undershoot
        }
    }
 
    stepper->speedcontrol(current_rpm); // Set motor speed

}

