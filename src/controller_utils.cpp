#include "controller_utils.h"

// Map function similar to Arduino's map()
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

ControllerUtil::ControllerUtil(IO *io, motor *stepper, LinearControl *handle_controller, ADS1232 *ads, UART *uart, int *button)
    : io(io), stepper(stepper), handle_controller(handle_controller), ads(ads), uart(uart), button(button) {}

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
    sprintf(buffer, "out rpm: %d, %.2f\n", speed, linear_value);  // Format output string
    uart->transmitString(buffer);  // Send filtered value over UART
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