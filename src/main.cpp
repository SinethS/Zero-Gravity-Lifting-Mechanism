#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "UART.h"


// defines

// defines end

// variable declarations
volatile bool loop_flag = false;

motor stepper(200);  // Initialize motor with 1600 microsteps
UART uart;  // Initialize UART

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

// function declarations end


int main(void) {

    uart.transmitString("hello world!");  // Send message over UART

    timer2_ctc_100hz_init();  // Initialize Timer2 for 100 Hz
    stepper.initMotor();  // Initialize motor
    uart.println("Motor initialized");  // Send message over UART


    while (1) {
        // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
        if(loop_flag) {
            loop_flag = false;  // Reset flag
            uart.println("Looping...");  // Send message over UART
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