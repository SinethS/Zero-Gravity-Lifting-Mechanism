#include "linearControl.h"

volatile uint16_t LinearControl::raw_value = 0;
volatile float LinearControl::filtered_value = 0.0f;

LinearControl::LinearControl() {
    // Constructor
    raw_value = 0;
    filtered_value = 0.0f;
}

void LinearControl::begin() {

    // Select AVcc as reference (REFS0 = 1), ADC8 = PK0
    ADMUX = (1 << REFS0);          // AVcc with external capacitor at AREF
    ADCSRB |= (1 << MUX5);         // Enable MUX5 for ADC8
    ADMUX |= (0 << MUX2) | (0 << MUX1) | (0 << MUX0); // MUX[3:0] = 0000 for ADC8

    // ADC enable, interrupt enable, prescaler 128 (16MHz / 128 = 125kHz ADC clock)
    ADCSRA = (1 << ADEN)  | (1 << ADIE) |
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);


    sei();  // Enable global interrupts

}

void LinearControl::start_conversion() {
    ADCSRA |= (1 << ADSC);
}

// Get the last raw ADC value
uint16_t LinearControl::get_raw(){
    return raw_value;
}

// Get the last filtered value
float LinearControl::get_filtered(){
    return filtered_value;
}

// ISR handler to call from ADC_vect
void ADC_vect_handler() {
    LinearControl::raw_value = ADC;
    LinearControl::filtered_value += LinearControl::alpha * (LinearControl::raw_value - LinearControl::filtered_value);
    ADCSRA |= (1 << ADSC); // Start next conversion
}

// Actual ISR
ISR(ADC_vect) {
    ADC_vect_handler();
}
