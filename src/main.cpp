#include <avr/io.h>
#include <util/delay.h>
#include <math.h>


// Define STEP and DIR pins
#define STEP_PIN  PH3
#define DIR_PIN  PH4

// Pulse timing
#define PULSE_WIDTH_US 500  // ≥ 2.5us for CL57T, we use 5us

// Steps per revolution for full step mode (1.8° stepper)
#define STEPS_PER_REV 200

void gpio_init() {
    // Set STEP and DIR pins as output
    DDRH |= (1 << STEP_PIN) | (1 << DIR_PIN);
}

void step_pulse() {
    PORTH |= (1 << STEP_PIN);             // STEP high
    _delay_us(PULSE_WIDTH_US);           // Hold high
    PORTH &= ~(1 << STEP_PIN);           // STEP low
    _delay_us(PULSE_WIDTH_US);           // Hold low (same as high)
}

void set_direction(uint8_t dir) {
    if (dir)
        PORTH |= (1 << DIR_PIN);  // Clockwise
    else
        PORTH &= ~(1 << DIR_PIN); // Counterclockwise
}

void rotate_stepper(float rpm, float angle, uint8_t dir) {
    // Set direction
    set_direction(dir);

    // Steps to rotate
    uint32_t steps;
    if (angle == 0) {
        steps = 0xFFFFFFFF;  // Infinite steps
    } else {
        steps = (uint32_t)((angle / 360.0) * STEPS_PER_REV);
    }

    // Delay between steps (in microseconds)
    float step_delay_us = (60.0 * 1000000.0) / (rpm * STEPS_PER_REV);

    while (steps--) {
        step_pulse();
        _delay_us((uint16_t)(step_delay_us - 2 * PULSE_WIDTH_US));  // Adjust for pulse time
    }
}

int main() {
    gpio_init();

    float rpm = 50.0;    // Change as needed
    float angle = 0.0;   // 0 = continuous rotation
    uint8_t direction = 1; // 1 = CW, 0 = CCW

    while (1) {
        rotate_stepper(rpm, angle, direction);

        // Optional delay before reversing
        _delay_ms(5000);

    }
}

//so pwm