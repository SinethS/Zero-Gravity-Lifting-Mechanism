#include "timer.h"

void setupTimer2() {
    TCCR2A = (1 << WGM21);  // CTC mode
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);  // Prescaler 1024
    OCR2A = 124;            // 125Hz -> (16MHz / (1024 * 125)) - 1 = 124
    TIMSK2 = (1 << OCIE2A); // Enable Compare Match A interrupt
    TCNT2 = 0;
}

// Timer3 - 2ms interval (500Hz)
void setupTimer3() {
    TCCR3A = 0; // CTC mode
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30); // Prescaler 64
    OCR3A = 499;            // 2ms -> (16MHz / (64 * 500)) - 1 = 499
    TIMSK3 |= (1 << OCIE3A); // Enable Compare Match A interrupt
    TCNT3 = 0;
    sei(); // Enable global interrupts (optional if done in main)
}

// Timer5 - 100ms interval (10Hz)
void setupTimer5() {
    TCCR5A = 0;
    TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50); // CTC mode, prescaler 1024
    OCR5A = 1562;              // 100ms -> (16MHz / (1024 * 10)) - 1 = 1562
    TIMSK5 = (1 << OCIE5A);    // Enable Compare Match A interrupt
    TCNT5 = 0;
}

// Timer2 interrupt handler

ISR(TIMER2_COMPA_vect) {
    loop_flag = true; // Set the loop flag
}
// Timer3 interrupt handler
ISR(TIMER3_COMPA_vect) {
  // Timer3 interrupt code here

  // ICM20948_read_accel_gyro(&accel, &gyro); // Read accelerometer and gyroscope data

  
}


// Timer5 interrupt handler
ISR(TIMER5_COMPA_vect) {
    // Timer5 interrupt code here
}

