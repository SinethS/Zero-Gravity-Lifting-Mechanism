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
#include "UI_utils.h"
#include "controller_utils.h"
#include "touchcontroller.h"

volatile bool loop_flag = false; // Flag for loop execution
char buffer[100]; // Buffer for formatted strings


// defines

// defines end

// variable declarations

int button = 0;


motor stepper(1600);      // Initialize motor with 1600 microsteps
UART uart(115200);        // Initialize UART
IO io;                    // Initialize IO buttons and LEDs
LinearControl controller; // Initialize LinearControl
ADS1232 ads(&PORTE, &DDRE, &PINE, PE5, PE4, PE6);
UIUtils ui_utils(&io, &button); // Initialize UI utilities
TouchController touchController; // Initialize touch controller
ControllerUtil controller_util(&io, &stepper, &controller, &ads, &touchController, &uart, &button); // Initialize controller utilities


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
    // function declarations end


// function declarations end

int main(void)
{
    // Initialize peripherals

    uart.transmitString("hello world!"); // Send message over UART

    timer2_ctc_100hz_init();                   // Initialize Timer2 for 100 Hz
    stepper.initMotor();                       // Initialize motor
    stepper.setSafetyCount(&eeprom); // Set safety count from EEPROM
    uart.print("Motor initialized - ");         // Send message over UART
    uart.println(stepper.getsafetyCount()); // Print initial safety coun
    millis_init();                             // Initialize millis
    uart.println("Millis initialized");        // Send message over UART
    io.initIO();                               // Initialize IO
    uart.println("IO initialized");            // Send message over UART
    ads.init();                                // Initialize ADS1232
    uart.println("ADS1232 initialized");       // Send message over UART
    controller.begin();                        // Initialize LinearControl
    uart.println("LinearControl initialized"); // Send message over UART
    menu_init();                              // Initialize display menu
    menu_update();                            // Update the menu display
    uart.println("Display menu initialized"); // Send message over UART

    stepper.stopMotor();

    controller_util.callibrateADS1232_weight(2500.0f); // Callibrate ADS1232 with a known weight
    touchController.updateInitial(ads.getAverage(100)); // Update initial touch value


    ads.attachInterrupt(); // Attach interrupt for ADS1232 data ready

    controller.start_conversion(); // Start ADC conversion



    while (1) {
        if (loop_flag) {
            loop_flag = false; // Clear loop flag

            // ui_utils.runMenu(); // Run UI utilities to handle button presses

            if(stepper.saveSafetyToEEPROM(&eeprom)){
                uart.println("Safety count saved to EEPROM"); // Notify if safety count is saved
            }

            // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
        }
    }
}

touchController.updateSpeed(ADS1232_Read());
            stepper.speedcontrol(touchController.getSpeed()); // Set motor speed based on touch controller

            stepper.trapspeedcontrol(-200, 0.01); // Trapezoidal speed control for the motor

            uart.println(touchController.getSpeed()); // Send message over UART