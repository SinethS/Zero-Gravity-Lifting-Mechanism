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
#include "controller_utils.h"
#include "touchcontroller.h"
#include "profilecontroller.h"

volatile bool loop_flag = false; // Flag for loop execution
char buffer[100];                // Buffer for formatted strings

// defines

// defines end

// variable declarations

int button = 0;

motor stepper(1600);                           // Initialize motor with 1600 microsteps
ProfileController profilecontroller(&stepper); // Initialize ProfileController with motor
UART uart(115200);                             // Initialize UART
IO io;                                         // Initialize IO buttons and LEDs
LinearControl controller;                      // Initialize LinearControl
ADS1232 ads(&PORTE, &DDRE, &PINE, PE5, PE4, PE6);
TouchController touchController(10000);                                                                        // Initialize touch controller
ControllerUtil controller_util(&io, &profilecontroller, &controller, &ads, &touchController, &uart, &button); // Initialize controller utilities
Menu menu(&io, &button, &controller_util);                                                                    // Initialize menu with IO and button state

EEPROMManager eeprom; // Initialize EEPROM manager

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

ISR(TIMER5_COMPA_vect)
{

    stepper.stopMotor(); // Stop motor on compare match
    uart.println("Motor stopped");
}

ISR(PCINT1_vect)
{
    io.detachINTERUPT_PCINT1(); // Detach interrupt for Port K
    int x = io.buttonUpdate();  // Update button state
    button = x;
    uart.println(x);             // Send button state over UART
    io.attacthINTERUPT_PCINT1(); // Reattach interrupt for Port K
}
// function declarations end

// function declarations end

int main(void)
{
    // Initialize peripherals

    uart.transmitString("hello world!"); // Send message over UART

    timer2_ctc_100hz_init();                   // Initialize Timer2 for 100 Hz
    stepper.initMotor();                       // Initialize motor
    stepper.setSafetyCount(&eeprom);           // Set safety count from EEPROM
    uart.print("Motor initialized - ");        // Send message over UART
    uart.println(stepper.getsafetyCount());    // Print initial safety coun
    millis_init();                             // Initialize millis
    uart.println("Millis initialized");        // Send message over UART
    io.initIO();                               // Initialize IO
    uart.println("IO initialized");            // Send message over UART
    ads.init();                                // Initialize ADS1232
    uart.println("ADS1232 initialized");       // Send message over UART
    controller.begin();                        // Initialize LinearControl
    uart.println("LinearControl initialized"); // Send message over UART
    menu.menu_init();       
    controller_util.addMenu(&menu);                   // Initialize display menu
    uart.println("Display menu initialized");  // Send message over UART

    profilecontroller.init();                      // Initialize profile controller
    uart.println("ProfileController initialized"); // Send message over UART

    stepper.stopMotor();

    menu.showCalibrationScreen(); // Show calibration screen on display
    

    controller_util.callibrateADS1232_weight(2500.0f);  // Callibrate ADS1232 with a known weight
    touchController.updateInitial(ads.getAverage(100)); // Update initial touch value



    ads.attachInterrupt(); // Attach interrupt for ADS1232 data ready

    controller.start_conversion(); // Start ADC conversion

    ads.attachInterrupt(); // Attach interrupt for ADS1232 data ready

    while (1)
    {
        if (loop_flag)
        {
            // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
            loop_flag = false; // Clear loop flag

            if (stepper.saveSafetyToEEPROM(&eeprom))
            {
                uart.println("Safety count saved to EEPROM"); // Notify if safety count is saved
            }

            menu.runMenu();         // Run the menu to handle button inputs and display updates
            menu.run_active_mode(); // Run the active mode (e.g., constant speed mode)

            // profilecontroller.run(-90); // Stop the profile controller

            // if (ads.getWeight() > 5000.0f)
            // {
            //     menu.showWarningScreen(); // Show warning screen if weight exceeds 5000 grams

            // }

            // if (ads.getWeight() > 7000.f){
            //     menu.showWarningScreen(); // Show warning screen if weight exceeds 7000 grams
            //     stepper.stopMotor(); // Stop motor if weight exceeds 7000 grams
                
            // }


            // controller_util.handleADS1232Control(); // Handle linear control input

            // uart.println("Looping...");  // Send message over UART

            // uint32_t data = ads.read(); // Read raw data from ADS1232
            // sprintf(buffer, "Raw data: %ld\n", data); // Format raw data
            // uart.transmitString(buffer); // Send raw data over UART

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