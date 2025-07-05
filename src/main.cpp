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

volatile bool loop_flag = false; // Flag for loop execution

// defines

// defines end

// variable declarations

motor stepper(1600);      // Initialize motor with 1600 microsteps
UART uart(115200);        // Initialize UART
IO io;                    // Initialize IO buttons and LEDs
LinearControl controller; // Initialize LinearControl
ADS1232 ads(&PORTE, &DDRE, &PINE, PE5, PE4, PE6);

int button = 0;

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
    // uart.transmitNumber(x);  // Send button state over UART
    io.attacthINTERUPT_PCINT1(); // Reattach interrupt for Port K
}

// // function declarations end

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
    // Initialize peripherals

    //     uart.transmitString("hello world!");  // Send message over UART

    timer2_ctc_100hz_init();                   // Initialize Timer2 for 100 Hz
    stepper.initMotor();                       // Initialize motor
    uart.println("Motor initialized");         // Send message over UART
    millis_init();                             // Initialize millis
    uart.println("Millis initialized");        // Send message over UART
    io.initIO();                               // Initialize IO
    uart.println("IO initialized");            // Send message over UART
    ads.init();                                // Initialize ADS1232
    uart.println("ADS1232 initialized");       // Send message over UART
    controller.begin();                        // Initialize LinearControl
    uart.println("LinearControl initialized"); // Send message over UART
    menu_init();                              // Initialize display menu
    uart.println("Display menu initialized"); // Send message over UART

    // stepper.speedcontrol(0);
    // // stepper.turnAngle(-3600, 60);  // Turn motor 360 degrees at 10 RPM
    // // stepper.runMotor();
    // stepper.stopMotor();
    // controller.start_conversion(); // Start ADC conversion

    // char buffer[100];

    // uart.println("Remove all weight from the scale...\n");
    // _delay_ms(10000); // Wait for 6 seconds to allow user to remove weight
    // uart.println("Starting calibration.");
    // ads.calibrate();               // Calibrate ADS1232 to find offset
    // uart.println(ads.getOffset()); // Print offset value
    // uart.println("Now place a known weight (e.g., 1000g) on the scale.\n");
    // _delay_ms(10000);
    // ads.CalcScale(2500.0);        // Scale calibration with known weight (e.g., 2500g)
    // uart.println(ads.getScale()); // Print scale value

    // ads.attachInterrupt(); // Attach interrupt for ADS1232 data ready

    while (1) {
        if (loop_flag) {
            loop_flag = false; // Clear loop flag

            // MODIFICATION: Check for and process button presses
            if (button != 0) {
                // We only care about the press event (positive value)
                if (button > 0) {
                    menu_process_button(button);
                }
                button = 0; // Consume the button event to prevent re-triggering
            }

            // MODIFICATION: menu_update now just draws the current state
            menu_update();

            // --- Other application logic can go here ---
            // float weight = ads.Weight();
            // ...
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