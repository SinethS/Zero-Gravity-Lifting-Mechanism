#ifndef SH1106_H
#define SH1106_H

#include <stdint.h>
#include "i2c.h" // Updated include

class SH1106 {
public:
    static constexpr uint8_t WIDTH = 128;
    static constexpr uint8_t HEIGHT = 64;
    static constexpr uint8_t PAGES = 8;

    // Constructor: Takes an I2C object reference and the device address
    SH1106(I2C& i2c, uint8_t address = 0x3C);

    void clear();
    void setPosition(uint8_t x, uint8_t page);
    void drawChar(char c, uint8_t x, uint8_t page);
    void drawString(const char* str, uint8_t x, uint8_t page);

private:
    void command(uint8_t cmd);
    void sendData(uint8_t data);

    I2C& m_i2c;       // Reference to the I2C bus object
    uint8_t m_address; // I2C address of the display
};

#endif // SH1106_H