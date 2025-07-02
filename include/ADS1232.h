#ifndef ADS1232_H
#define ADS1232_H

#include <avr/io.h>
#include <util/delay.h>

class ADS1232 {
  public:
    ADS1232(volatile uint8_t* port, volatile uint8_t* ddr, volatile uint8_t* pin,
            uint8_t sclk_pin, uint8_t dout_pin, uint8_t power_pin);

    void init();
    uint32_t read();
    void startConversion();
    void setGain(uint8_t gain);
    uint32_t getAverage(uint8_t samples);
    void calibrate(uint32_t* offset);
    void scale(uint32_t* scale, float known_weight, uint32_t offset);
    void Weight(uint32_t* weight, uint32_t scale, uint32_t offset);

  private:
    // Kalman filter state
    float q = 0.1f;
    float r = 0.1f;
    float x_hat = 0;
    float p = 1;
    float k;

    // Pin control
    volatile uint8_t* _port;
    volatile uint8_t* _ddr;
    volatile uint8_t* _pin;
    uint8_t _sclk;
    uint8_t _dout;
    uint8_t _power;

    void pulseClock();
};

#endif
