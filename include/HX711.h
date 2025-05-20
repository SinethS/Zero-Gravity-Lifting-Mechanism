#ifndef HX711_AVR_H
#define HX711_AVR_H

#include <avr/io.h>
#include <util/delay.h>

class HX711 {
public:
    // Constructor with pin numbers (assumes PORTB for AVR)
    HX711();
    void init_HX711(uint8_t dataPin, uint8_t clockPin, uint8_t gain = 128);
    
    bool is_ready();
    long read();
    long read_average(uint8_t times = 10);
    double get_value(uint8_t times = 1);
    float get_units(uint8_t times = 1);
    void tare(uint8_t times = 10);
    void set_scale(float scale = 1.f);
    float get_scale();
    void set_offset(long offset = 0);
    long get_offset();
    void power_down();
    void power_up();
    long get_raw_value();

    void attach_interrupt();
    void detach_interrupt();

private:
    uint8_t _dataPin;
    uint8_t _clockPin;
    uint8_t _gain;
    long _offset;
    float _scale;
    volatile uint8_t* _dataDdr;
    volatile uint8_t* _dataPort;
    volatile uint8_t* _dataPinReg;
    volatile uint8_t* _clockDdr;
    volatile uint8_t* _clockPort;

    long raw_value;
    float value;

};

#endif