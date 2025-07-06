#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "UART.h"
#include "IO.h"
#include "timemillis.h"
#include "I2C.h"
#include "linearControl.h"
#include "ADS1232.h"
#include "menu.h"
// #include "UI_utils.h"
#include "controller_utils.h"

volatile bool loop_flag = false; // Flag for loop execution

// defines

// defines end

// variable declarations

int button = 0;


motor stepper(1600);      // Initialize motor with 1600 microsteps
UART uart(115200);        // Initialize UART
IO io;                    // Initialize IO buttons and LEDs
LinearControl controller; // Initialize LinearControl
ADS1232 ads(&PORTE, &DDRE, &PINE, PE5, PE4, PE6);
// UIUtils ui_utils(&io, &button); // Initialize UI utilities
ControllerUtil controller_util(&io, &stepper, &controller, &ads, &uart, &button); // Initialize controller utilities
Menu menu(&io, &button, &controller_util); // Initialize menu with IO and button state

EEPROMManager eeprom; // Initialize EEPROM manager


int mode = 0;
int prev_button = 0; // Previous button state for mode selection

// // varible declarations end

// // function declarations

ISR(INT4_vect)
{
    ads.detachInterrupt();                 // Detach interrupt to prevent re-entrance
    bool ads_data_ready = ads.dataReady(); // Check if HX711 data is ready
    if (ads_data_ready)
    {
        ads.read(); // Read raw value from HX711
    }
    ads.attachInterrupt(); // Detach interrupt to prevent re-entrance
}

ISR(TIMER2_COMPA_vect)
{
    loop_flag = true;        // Set flag every 8 ms
    stepper.motorSafetyEN(); // Enable motor safety feature
}

ISR(TIMER5_COMPA_vect) {
    
    stepper.stopMotor();  // Stop motor on compare match
    uart.println("Motor stopped");
}

ISR(PCINT1_vect)
{
    io.detachINTERUPT_PCINT1(); // Detach interrupt for Port K
    int x = io.buttonUpdate();  // Update button state
    button = x;
    // create and put update display or get button input display
    uart.println(x);  // Send button state over UART
    io.attacthINTERUPT_PCINT1(); // Reattach interrupt for Port K
}


// // function declarations end

int main(void)
{
    // Initialize peripherals

    uart.transmitString("hello world!");  // Send message over UART

    timer2_ctc_100hz_init();                   // Initialize Timer2 for 100 Hz
    stepper.initMotor();                       // Initialize motor
    stepper.setSafetyCount(&eeprom); // Set safety count from EEPROM
    uart.print("Motor initialized - ");         // Send message over UART
    uart.println(stepper.getsafetyCount()); // Print initial safety count
    millis_init();                             // Initialize millis
    uart.println("Millis initialized");        // Send message over UART
    io.initIO();                               // Initialize IO
    uart.println("IO initialized");            // Send message over UART
    ads.init();                                // Initialize ADS1232
    uart.println("ADS1232 initialized");       // Send message over UART
    // controller.begin();                        // Initialize LinearControl
    uart.println("LinearControl initialized"); // Send message over UART
    // menu.menu_init();                              // Initialize display menu
    uart.println("Display menu initialized"); // Send message over UART

    // stepper.speedcontrol(0);
    // stepper.turnAngle(-3600, 60);  // Turn motor 360 degrees at 10 RPM
    // stepper.runMotor();
    stepper.stopMotor();
    controller.start_conversion(); // Start ADC conversion

    // uart.println("Remove all weight from the scale...\n");
    // _delay_ms(1000); // Wait for 6 seconds to allow user to remove weight
    // uart.println("Starting calibration.");
    // ads.calibrate();               // Calibrate ADS1232 to find offset
    // uart.println(ads.getOffset()); // Print offset value
    // uart.println("Now place a known weight (e.g., 1000g) on the scale.\n");
    // _delay_ms(1000);
    // ads.CalcScale(2500.0);        // Scale calibration with known weight (e.g., 2500g)
    // uart.println(ads.getScale()); // Print scale value

    // ads.attachInterrupt(); // Attach interrupt for ADS1232 data ready 

    int press = 0;

    while (1) {
        if (loop_flag) {
            loop_flag = false; // Clear loop flag
            // menu.runMenu(); // Run the menu to handle button presses
            // menu.run_active_mode(); // Run the active mode (e.g., constant speed mode)
            press = io.buttonUpdate(); // Update button state
            if (press != 0) {
                uart.print("Button pressed: ");
                uart.println(press); // Print button press over UART
                if (press == 1) {
                    io.controlLEDs(0b1000, true); // Turn on LED 0
                } else if (press == 2) {
                    io.controlLEDs(0b0100, true); // Turn on LED 1
                } else if (press == 3) {
                    io.controlLEDs(0b0010, true); // Turn on LED 2
                } else if (press == 4) {
                    io.controlLEDs(0b0001, true); // Turn on LED 3
                } else {
                    io.controlLEDs(0b0000, true); // Turn off all LEDs
                }
            }
            

            if(stepper.saveSafetyToEEPROM(&eeprom)){
                uart.println("Safety count saved to EEPROM"); // Notify if safety count is saved
            }

            // controller_util.handlLinearControl(); // Handle linear control input

            

            // uart.println("Looping...");  // Send message over UART

            // float weight = ads.Weight();  // Convert raw data to weight
            // sprintf(buffer,"Measured weight: %.2f grams\n", weight);
            // uart.transmitString(buffer);  // Send measured weight over UART

            // Print result
            // sprintf(buffer, "Data: %ld\n", data);  // Format
            // uart.transmitString(buffer);  // Send data over UART

            //             // // sprintf(buffer, "Time: %lu ms\n", millis());  // Get current time in milliseconds
            //             // // uart.transmitString(buffer);  // Send time over UART
     

            // loop code end
        }
    }
}

/*
EXAMPLE USAGE OF EEPROMManager

EEPROMManager eepromManager;

int main()
{
    int motor_speed = 1234; // Example value to store
    uart.transmitString("EEPROM Manager Demo\n");

    uint16_t val;
    if (eepromManager.read("motor_speed", &val))
    {
        uart.transmitString("Restored motor_speed: ");
        uart.transmitNumber(val);
        uart.transmitString("\n");
    }
    else
    {
        uart.transmitString("No motor_speed found in EEPROM.\n");
    }

    // Try to write the same value(will skip write if no change) if (eepromManager.storeIfChanged("motor_speed", motor_speed))
    {
        uart.transmitString("motor_speed is stored or already up to date.\n");
    }

    while (1)
    {
        _delay_ms(500);
        if (eepromManager.read("motor_speed", &val))
        {
            uart.transmitString("Loop read motor_speed: ");
            uart.transmitNumber(val);
            uart.transmitString("\n");
        }

        _delay_ms(1000);   // Wait before next update
        motor_speed += 10; // Increment value for next iteration

        if (eepromManager.storeIfChanged("motor_speed", motor_speed))
        {
            uart.transmitString("motor_speed updated to: ");
            uart.transmitNumber(motor_speed);
            uart.transmitString("\n");
        }
    }
}
*/