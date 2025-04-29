#include "HX711.h"


HX711::HX711(uint8_t dataPin, uint8_t clockPin, uint8_t gain) :
    _dataPin(dataPin),
    _clockPin(clockPin),
    _gain(gain),
    _offset(0),
    _scale(1.0f)
{
    // Initialize pointers to PORTB registers (AVR specific)
    _dataDdr = &DDRE;
    _dataPort = &PORTE;
    _dataPinReg = &PINE;
    _clockDdr = &DDRE;
    _clockPort = &PORTE;
    
    // Set clock pin as output
    *_clockDdr |= (1 << _clockPin);
    // Set data pin as input
    *_dataDdr &= ~(1 << _dataPin);
    
    // Initialize clock high
    *_clockPort |= (1 << _clockPin);
    _delay_us(100);
    *_clockPort &= ~(1 << _clockPin);
    
    // Set the gain and channel
    read();
}

bool HX711::is_ready() {
    return !(*_dataPinReg & (1 << _dataPin));
}

long HX711::read() {
    // Wait for the chip to become ready
    while (!is_ready()) {
        // Yield-like delay
        _delay_us(1);
    }
    
    unsigned long value = 0;
    uint8_t data[3] = { 0 };
    uint8_t filler = 0x00;
    
    // Pulse the clock pin 24 times to read the data
    for (uint8_t i = 0; i < 24; i++) {
        *_clockPort |= (1 << _clockPin);
        _delay_us(1);
        
        if (i < 8) {
            data[2] = data[2] << 1 | ((*_dataPinReg & (1 << _dataPin)) ? 1 : 0);
        } else if (i < 16) {
            data[1] = data[1] << 1 | ((*_dataPinReg & (1 << _dataPin)) ? 1 : 0);
        } else {
            data[0] = data[0] << 1 | ((*_dataPinReg & (1 << _dataPin)) ? 1 : 0);
        }
        
        *_clockPort &= ~(1 << _clockPin);
        _delay_us(1);
    }
    
    // Set the channel and the gain factor for the next reading
    for (uint8_t i = 0; i < _gain; i++) {
        *_clockPort |= (1 << _clockPin);
        _delay_us(1);
        *_clockPort &= ~(1 << _clockPin);
        _delay_us(1);
    }
    
    // Replicate the most significant bit to pad out a 32-bit signed integer
    if (data[2] & 0x80) {
        filler = 0xFF;
    } else {
        filler = 0x00;
    }
    
    // Construct a 32-bit signed integer
    value = ( static_cast<unsigned long>(filler) << 24
          | static_cast<unsigned long>(data[2]) << 16
          | static_cast<unsigned long>(data[1]) << 8
          | static_cast<unsigned long>(data[0]));
    
    return static_cast<long>(value);
}

long HX711::read_average(uint8_t times) {
    long sum = 0;
    for (uint8_t i = 0; i < times; i++) {
        sum += read();
    }
    return sum / times;
}

double HX711::get_value(uint8_t times) {
    return read_average(times) - _offset;
}

float HX711::get_units(uint8_t times) {
    return get_value(times) / _scale;
}

void HX711::tare(uint8_t times) {
    double sum = read_average(times);
    set_offset(sum);
}

void HX711::set_scale(float scale) {
    _scale = scale;
}

float HX711::get_scale() {
    return _scale;
}

void HX711::set_offset(long offset) {
    _offset = offset;
}

long HX711::get_offset() {
    return _offset;
}

void HX711::power_down() {
    *_clockPort &= ~(1 << _clockPin);
    *_clockPort |= (1 << _clockPin);
}

void HX711::power_up() {
    *_clockPort &= ~(1 << _clockPin);
}

