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

volatile bool loop_flag = false;  // Flag for loop execution


// defines

// defines end

// variable declarations

motor stepper(1600);  // Initialize motor with 1600 microsteps
UART uart(115200);  // Initialize UART
IO io; // Initialize IO buttons and LEDs
LinearControl controller;  // Initialize LinearControl

int button = 0;
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

ISR(TIMER2_COMPA_vect) {
    loop_flag = true;  // Set flag every 8 ms
    stepper.motorSafetyEN();  // Enable motor safety feature
}

ISR(TIMER5_COMPA_vect) {
    stepper.stopMotor();  // Stop motor on compare match
    uart.println("Motor stopped"); 
}

ISR(PCINT1_vect){
    io.detachINTERUPT_PCINT1();  // Detach interrupt for Port K
    int x = io.buttonUpdate();  // Update button state
    button = x;
    // create and put update display or get button input display
    // uart.transmitNumber(x);  // Send button state over UART
    io.attacthINTERUPT_PCINT1();  // Reattach interrupt for Port K
}

// function declarations end

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

    stepper.speedcontrol(0);
    // stepper.turnAngle(-3600, 60);  // Turn motor 360 degrees at 10 RPM
    // stepper.runMotor();
    stepper.stopMotor();
    controller.start_conversion();  // Start ADC conversion

      char buffer[100];
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
        if(loop_flag) {  // Check if loop flag is set
            loop_flag = false;  // Clear loop flag

            // stepper.speedcontrol(0);  // Stop motor
            // while(button == 3){
            //     stepper.speedcontrol(30);
            //     sprintf(buffer, "%ld, %u\n", stepper.getsafetyCount(), TCNT5);  // Format safety count
            //     uart.transmitString(buffer);  // Send safety count over UART
            //     io.controlLEDs(0b1000, true);  // Turn on LED 0
            //     _delay_ms(8);        
            // }
            // stepper.speedcontrol(0);  // Stop motor
            // while(button == 2){

            //     stepper.speedcontrol(-30);
            //     sprintf(buffer, "%ld, %u\n", stepper.getsafetyCount(), TCNT5);  // Format safety count
            //     uart.transmitString(buffer);  // Send safety count over UART
            //     io.controlLEDs(0b0100, true);  // Turn on LED 1
            //     _delay_ms(8);
            // }
            // stepper.speedcontrol(0);  // Stop motor
            // io.controlLEDs(0b0000, true);  // Turn on LED 1
            



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
    } // // sprintf(buffer, "Time: %lu ms\n", millis());  // Get current time in milliseconds
            // // uart.transmitString(buffer);  // Send time over UART
            // float x = controller.get_filtered();
            // sprintf(buffer, "Filtered Value: %.2f\n", x);  // Format filtered value
            // uart.transmitString(buffer);  // Send filtered value over UART
            // _delay_ms(100);  // Delay for 50 ms

            // x = pow((x-10)/830, 0.5)*1000            
            // // x = x/1024*1000;  // Scale the filtered value
            // int speed = (int)x;  // Convert to integer

            // speed = map(speed, 0, 1000, -150, 150);  // Map the speed value to a range

            // if(abs(speed - prv_speed) < 5) {  // Check if speed change is significant
            //     speed = prv_speed;  // Use previous speed if change is small
            // }
            
            // prv_speed = speed;  // Update previous speed
            // stepper.speedcontrol(speed);  // Control motor speed based on filtered value
            // sprintf(buffer, "out rpm: %d, %.2f\n", speed, x);  // Format output string
            // uart.transmitString(buffer);  // Send filtered value over UART
            // sprintf(buffer, "Raw Value: %ld\n", hx711.get_raw_value());  // Get raw value from HX711
            // uart.transmitString(buffer);  // Send raw value over UART
            // sprintf(buffer, "Filtered Value: %.2f\n", controller.get_filtered());  // Get filtered value from LinearControl
            // uart.transmitString(buffer);  // Send filtered value over UART
            
            // loop code end
}
