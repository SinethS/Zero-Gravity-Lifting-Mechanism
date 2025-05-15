#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "UART.h"
#include "IO.h"
#include "io_portk.h"

// defines

// defines end

// variable declarations
volatile bool loop_flag = false;

motor stepper(1600);  // Initialize motor with 1600 microsteps
UART uart;  // Initialize UART
// IO io;  // Initialize IO
IO_PortK io_k;

// varible declarations end


// function declarations
void timer2_ctc_100hz_init(void);

ISR(TIMER2_COMPA_vect) {
    loop_flag = true;  // Set flag every 8 ms
}

ISR(TIMER5_COMPA_vect) {
    stepper.stopMotor();  // Stop motor on compare match
    uart.println("Motor stopped"); 
}

ISR(PCINT2_vect){
    int x = io_k.buttonUpdate();  // Update button state
    // create and put update display or get button input display
    uart.transmitNumber(x);  // Send button state over UART
}

// function declarations end


int main(void) {

    uart.transmitString("hello world!");  // Send message over UART

    timer2_ctc_100hz_init();  // Initialize Timer2 for 100 Hz
    stepper.initMotor();  // Initialize motor
    uart.println("Motor initialized");  // Send message over UART

    io_k.initIO();  // Initialize IO
    uart.println("IO initialized");  // Send message over UART

    // stepper.speedcontrol(-100);  // Set speed to 100 RPM




    while (1) {
        // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
        if(loop_flag) {
            loop_flag = false;  // Reset flag
            // uart.println("Looping...");  // Send message over UART
            // char buffer[50];
            // snprintf(buffer, sizeof(buffer), "Angle: %.2f \n", stepper.getAngle());  // Get angle from motor
            // uart.transmitString(buffer);  // Send angle over UART
            io_k.controlLEDs(0x0F, true);  // Set all LEDs on Port K
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0x00, true);  // Turn off all LEDs
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(1, false);  // Toggle all LEDs on Port K
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(2, false);  // Turn off all LEDs
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(3, false);  // Toggle all LEDs on Port K
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(4, false);  // Turn off all LEDs
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0b1111, false);  // Toggle all LEDs on Port K
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0b1110, false);  // Turn off all LEDs
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0b1100, false);  // Toggle all LEDs on Port K
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0b1000, false);  // Turn off all LEDs
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0b0001, false);  // Toggle all LEDs on Port 
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0b0010, false);  // Turn off all LEDs
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0b0100, false);  // Toggle all LEDs on Port K
            _delay_ms(100);  // Delay for 100 ms
            io_k.controlLEDs(0b1000, false);  // Turn off all LEDs
            _delay_ms(100);  // Delay for 100 ms



            // loop code begin

            // loop code end
        }
    }
}


void timer2_ctc_100hz_init(void) {
    TCCR2A = (1 << WGM21);  // CTC mode
    TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22);  // 1024

    TIMSK2 = (1 << OCIE2A);  // Enable Timer2 compare interrupt

    // (F_CPU / (Prescaler * Frequency)) - 1 = (16e6 / (1024 * 125)) - 1 = 124
    OCR2A = 124; // Set compare value for 125 Hz
}