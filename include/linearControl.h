#ifndef ADC_READER_H
#define ADC_READER_H

#include <avr/io.h>
#include <avr/interrupt.h>

class LinearControl {

public:
    LinearControl(); // Default constructor
    void begin();                 // Call once in setup
    void start_conversion();      // Call this inside your 1ms timer interrupt
    uint16_t get_raw();     // Get last raw ADC value
    float get_filtered();   // Get low-pass filtered value

private:
    static volatile uint16_t raw_value;
    static volatile float filtered_value;
    static constexpr float alpha = 0.1f;  // Low-pass filter strength

    friend void ADC_vect_handler();       // Friend ISR handler
};

void ADC_vect_handler(); // Call this inside ISR(ADC_vect)

#endif
