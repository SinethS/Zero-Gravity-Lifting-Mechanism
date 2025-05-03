# include "motor.h"

motor::motor(unsigned long microstep) : microstep(microstep) {
    // Constructor
}

void motor::initMotor() {
    initPWM_TIM1();
    initCounter_TIM5();
}

void motor::initPWM_TIM1() {
    // Set OC1A (PB5) as output
    DDRB |= (1 << PB5);

    // Set CTC mode: WGM12 = 1 (Mode 4), toggle OC1A on match
    TCCR1A = (1 << COM1A0);                     // Toggle OC1A on compare match
    TCCR1B = (1 << WGM12) | (1 << CS11);        // CTC mode, Prescaler = 8

    // Calculate and set OCR1A for desired frequency
    // OCR1A = (F_CPU / (2 * PRESCALER * frequency)) - 1;
}

void motor::initCounter_TIM5() {
    // Set T5 (PL2) as input
    DDRL &= ~(1 << PL2);  // Pin 47 as input

    // Clear Timer on Compare Match mode (CTC)
    TCCR5B = (1 << WGM52);  // CTC with OCR5A

    // External clock source on rising edge
    TCCR5B |= (1 << CS52) | (1 << CS51) | (1 << CS50);  // 111 = external clock on rising edge of T5

    // Set compare value (e.g., interrupt after 100 rising edges)
    OCR5A = 99;  // Zero-indexed, so 0–99 = 100 counts

    // Enable compare match interrupt
    TIMSK5 |= (1 << OCIE5A);

    // Clear timer
    TCNT5 = 0;

    sei();  // Enable global interrupts
}

void motor::setSpeed(int rpm) {
    if (rpm < 0) {
        rpm = 0;
    } else if (rpm > 255) {
        rpm = 255;
    }
    uint32_t frequency_hz = rpm; // Set the frequency based on RPM
    uint16_t ocr_val = (F_CPU / (2 * PRESCALER * frequency_hz)) - 1;
    OCR1A = ocr_val; // Set the PWM duty cycle
}

void motor::setDirection(bool dir) {

}

void motor::setAngle(int angle) {
    // Set the angle of the motor (not implemented in this example)
    uint32_t count = angle; // Set the count for the timer
    OCR5A = count - 1;  // Zero-indexed, so subtract 1
}