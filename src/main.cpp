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


// defines

// defines end

// variable declarations

motor stepper(1600);  // Initialize motor with 1600 microsteps
UART uart(115200);  // Initialize UART
IO io;
LinearControl controller;  // Initialize LinearControl
ADS1232 ads(&PORTE, &DDRE, &PINE, PE5, PE4, PE6); 

// varible declarations end


// function declarations

// ISR(INT4_vect) {
//     hx711.detach_interrupt();  // Detach interrupt to prevent re-entrance
//     bool hx711_data_ready = hx711.is_ready();  // Check if HX711 data is ready
//     if(hx711_data_ready) {
//         hx711.read();  // Read raw value from HX711
//     }

//     hx711.attach_interrupt();  // Detach interrupt to prevent re-entrance
// }

ISR(TIMER5_COMPA_vect) {
    stepper.stopMotor();  // Stop motor on compare match
    uart.println("Motor stopped"); 
}

ISR(PCINT1_vect){
    io.detachINTERUPT_PCINT1();  // Detach interrupt for Port K
    int x = io.buttonUpdate();  // Update button state
    // create and put update display or get button input display
    // uart.transmitNumber(x);  // Send button state over UART
    io.attacthINTERUPT_PCINT1();  // Reattach interrupt for Port K
}

// function declarations end

// Map function similar to Arduino's map()
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int main(void) {
    // Initialize peripherals
    

    uart.transmitString("hello world!");  // Send message over UART

    timer2_ctc_100hz_init();  // Initialize Timer2 for 100 Hz
    stepper.initMotor();  // Initialize motor
    uart.println("Motor initialized");  // Send message over UART
    millis_init();  // Initialize millis
    uart.println("Millis initialized");  // Send message over UART
    io.initIO();  // Initialize IO
    uart.println("IO initialized");  // Send message over UART
    ads.init();  // Initialize ADS1232
    uart.println("ADS1232 initialized");  // Send message over UART
    controller.begin();  // Initialize LinearControl
    uart.println("LinearControl initialized");  // Send message over UART

    // stepper.speedcontrol(0);
    // stepper.ENmotor();
    controller.start_conversion();  // Start ADC conversion

    int prv_speed = 0;  // Previous speed

    
    uint32_t offset = 0;
    uint32_t scale = 0;
    uint32_t weight = 0;

    uart.println("Remove all weight from the scale...\n");
    // wait for user to remove weight if needed
    // Wait for 60 seconds to allow user to remove weight
    uart.println("Starting calibration.");
    ads.calibrate(&offset);  // Calibrate ADS1232 to find offset
    uart.println(offset);

    _delay_ms(20000);  // Wait for 1 second before next step
    ads.scale(&scale, 5000.0, offset);  // Scale calibration with known weight (e.g., 2500g)
    uart.println(scale);


    uart.println("Now place a known weight (e.g., 1000g) on the scale.\n");
    _delay_ms(60000);  // Wait for 20 seconds to allow user to place weight
    // wait for user to place weight
    // Use the calibration to read and convert future weights




    while (1) {
        // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
        if(get_flag()) {  // Check if loop flag is set
            clear_flag();  // Clear loop flag
            // uart.println("Looping...");  // Send message over UART

            char buffer[50];

            long data = ads.getAverage(10); // Read data from ADS1232
            ads.Weight(&weight, scale, offset);  // Convert raw data to weight
            sprintf(buffer,"Measured weight: %.2f grams\n", weight);
            uart.transmitString(buffer);  // Send measured weight over UART

            // Print result
            sprintf(buffer, "Data: %ld\n", data);  // Format
            uart.transmitString(buffer);  // Send data over UART
            
            // loop code end
        }
    }
}
