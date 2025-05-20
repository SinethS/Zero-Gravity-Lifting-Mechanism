#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "UART.h"
#include "IO.h"
#include "io_portk.h"
#include "timemillis.h"

// defines

// defines end

// variable declarations

motor stepper(1600);  // Initialize motor with 1600 microsteps
UART uart;  // Initialize UART
// IO io;  // Initialize IO
IO_PortK io_k;

// varible declarations end


// function declarations


ISR(TIMER5_COMPA_vect) {
    stepper.stopMotor();  // Stop motor on compare match
    uart.println("Motor stopped"); 
}

ISR(PCINT2_vect){
    io_k.detachInterrupt_PCINTK();  // Detach interrupt for Port K
    int x = io_k.buttonUpdate();  // Update button state
    // create and put update display or get button input display
    uart.transmitNumber(x);  // Send button state over UART
    io_k.attachInterrupt_PCINTK();  // Reattach interrupt for Port K
}

// function declarations end


int main(void) {

    uart.transmitString("hello world!");  // Send message over UART

    timer2_ctc_100hz_init();  // Initialize Timer2 for 100 Hz
    stepper.initMotor();  // Initialize motor
    uart.println("Motor initialized");  // Send message over UART
    millis_init();  // Initialize millis
    uart.println("Millis initialized");  // Send message over UART
    io_k.initIO();  // Initialize IO
    uart.println("IO initialized");  // Send message over UART
    stepper.speedcontrol(60);
    stepper.ENmotor();





    while (1) {
        // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
        if(get_flag()) {  // Check if loop flag is set
            clear_flag();  // Clear loop flag
            // uart.println("Looping...");  // Send message over UART
            // char buffer[50];
            // sprintf(buffer, "Time: %lu ms\n", millis());  // Get current time in milliseconds
            // uart.transmitString(buffer);  // Send time over UART
            // snprintf(buffer, sizeof(buffer), "Angle: %.2f \n", stepper.getAngle());  // Get angle from motor
            // uart.transmitString(buffer);  // Send angle over UART
            // io_k.controlLEDs(0x0F, true);  // Set all LEDs on Port K
            // _delay_ms(1000);  // Delay for 100 ms
            // io_k.controlLEDs(0x00, true);  // Turn off all LEDs
            // _delay_ms(1000);  // Delay for 100 ms
            // io_k.controlLEDs(1, false);  // Toggle all LEDs on Port K
            // _delay_ms(1000);  // Delay for 100 ms
            // io_k.controlLEDs(1, false);
            // io_k.controlLEDs(2, false);  // Turn off all LEDs
            // _delay_ms(1000);  // Delay for 100 ms
            // io_k.controlLEDs(2, false);  // Turn off all LED
            // io_k.controlLEDs(3, false);  // Toggle all LEDs on Port K
            // _delay_ms(1000);  // Delay for 100 ms
            // io_k.controlLEDs(3, false);  // Toggle all LEDs on Port K
            // io_k.controlLEDs(4, false);  // Turn off all LEDs
            // _delay_ms(1000);  // Delay for 100 ms
            // io_k.controlLEDs(0b1111, true);  // Toggle all LEDs on Port K
            // _delay_ms(100);  // Delay for 100 ms
            // io_k.controlLEDs(0b1110, true);  // Turn off all LEDs
            // _delay_ms(100);  // Delay for 100 ms
            // io_k.controlLEDs(0b1100, true);  // Toggle all LEDs on Port K
            // _delay_ms(100);  // Delay for 100 ms
            // io_k.controlLEDs(0b1000, true);  // Turn off all LEDs
            // _delay_ms(100);  // Delay for 100 ms
            // io_k.controlLEDs(0b0001, true);  // Toggle all LEDs on Port 
            // _delay_ms(100);  // Delay for 100 ms
            // io_k.controlLEDs(0b0010, true);  // Turn off all LEDs
            // _delay_ms(100);  // Delay for 100 ms
            // io_k.controlLEDs(0b0100, true);  // Toggle all LEDs on Port K
            // _delay_ms(100);  // Delay for 100 ms
            // io_k.controlLEDs(0b1000, true);  // Turn off all LEDs
            // _delay_ms(100);  // Delay for 100 ms



            // loop code begin

            // loop code end
        }
    }
}
