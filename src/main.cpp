#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "UART.h"


// defines
#define PRESCALER 8

// defines end

// variable declarations
volatile bool loop_flag = false;
volatile uint32_t match_event_count = 0;

motor stepper(200);  // Initialize motor with 1600 microsteps
UART uart;  // Initialize UART

// varible declarations end


// function declarations
void timer2_ctc_100hz_init(void) {}

ISR(TIMER2_COMPA_vect) {
    loop_flag = true;  // Set flag every 8 ms
}

ISR(TIMER5_COMPA_vect) {
    stepper.stopMotor();  // Stop motor on compare match
}

// function declarations end


int main(void) {

    timer2_ctc_100hz_init();  // Initialize Timer2 for 100 Hz
    stepper.initMotor();  // Initialize motor
    stepper.setSpeed(250);  // Set motor speed to 100 RPM
    uart.transmitString("hello world!");  // Send message over UART


    while (1) {
        // Loop forever — frequency generation is hardware-driven
        if(loop_flag) {
            loop_flag = false;  // Reset flag
            uart.println("8 ms elapsed");  // Send message over UART
            uart.transmitNumber(match_event_count);  // Send match count over UART
        }
    }
}



void timer2_ctc_100hz_init(void) {
    TCCR2A = (1 << WGM21);  // CTC mode
    TCCR2B = (1 << CS20) | (1 << CS21) | (1 << CS22);  // 64

    // (F_CPU / (Prescaler * Frequency)) - 1 = (16e6 / (64 * 100)) - 1 = 249
    OCR2A = 124; // Set compare value for 125 Hz



