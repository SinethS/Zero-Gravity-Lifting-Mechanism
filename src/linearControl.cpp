#include "linearControl.h"

volatile uint16_t LinearControl::raw_value = 0;
volatile float LinearControl::filtered_value = 0.0f;

LinearControl::LinearControl() {
    // Constructor
    // Initialize raw_value and filtered_value to 0
    raw_value = 0;
    filtered_value = 0.0f;
}

void LinearControl::begin() {

    // AVcc reference + ADC1 (PF1)
    ADMUX = (1 << REFS0) | (1); // ADC1 = PF1

    // ADC enable, interrupt enable, prescaler 128 (125kHz ADC clock)
    ADCSRA = (1 << ADEN)  | (1 << ADIE) |
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

}

void LinearControl::start_conversion() {
    ADCSRA |= (1 << ADSC);
}

uint16_t LinearControl::get_raw(){
    return raw_value;
}

float LinearControl::get_filtered(){
    return filtered_value;
}

// ISR handler to call from ADC_vect
void ADC_vect_handler() {
    LinearControl::raw_value = ADC;
    LinearControl::filtered_value += LinearControl::alpha * (LinearControl::raw_value - LinearControl::filtered_value);
    // Directly start next conversion by setting ADSC bit
    ADCSRA |= (1 << ADSC); // Start next conversion
}

// Actual ISR
ISR(ADC_vect) {
    ADC_vect_handler();
}
