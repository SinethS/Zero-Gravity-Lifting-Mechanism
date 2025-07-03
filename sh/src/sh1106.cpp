#include "sh1106.h" // Updated include
#include "font.h"
#include <avr/pgmspace.h>
#include <util/delay.h>

// Constructor: Initializes the display
SH1106::SH1106(I2C& i2c, uint8_t address) : m_i2c(i2c), m_address(address) {
    _delay_ms(100);

    command(0xAE); // Display OFF
    command(0xD5); command(0x80); // Set Display Clock Divide Ratio
    command(0xA8); command(0x3F); // Set Multiplex Ratio (64)
    command(0xD3); command(0x00); // Set Display Offset
    command(0x40); // Set Start Line (0)
    command(0x8D); command(0x14); // Charge Pump Setting (Enable)
    command(0xA1); // Set Segment Re-map
    command(0xC8); // Set COM Output Scan Direction
    command(0xDA); command(0x12); // Set COM Pins Hardware Configuration
    command(0x81); command(0xCF); // Set Contrast
    command(0xD9); command(0xF1); // Set Pre-charge Period
    command(0xDB); command(0x40); // Set VCOMH
    command(0xA4); // Entire Display ON (resume)
    command(0xA6); // Normal Display
    command(0xAF); // Display ON
    _delay_ms(100);
}

void SH1106::command(uint8_t cmd) {
    m_i2c.start();
    m_i2c.write((m_address << 1) | 0);
    m_i2c.write(0x00);
    m_i2c.write(cmd);
    m_i2c.stop();
}

void SH1106::sendData(uint8_t data) {
    m_i2c.start();
    m_i2c.write((m_address << 1) | 0);
    m_i2c.write(0x40);
    m_i2c.write(data);
    m_i2c.stop();
}

void SH1106::clear() {
    for (uint8_t page = 0; page < PAGES; page++) {
        setPosition(0, page);
        for (uint8_t x = 0; x < WIDTH; x++) {
            sendData(0x00);
        }
    }
}

void SH1106::setPosition(uint8_t x, uint8_t page) {
    x += 2; // Column offset for 132x64 memory display
    command(0xB0 + page);
    command(0x10 | (x >> 4));
    command(0x00 | (x & 0x0F));
}

void SH1106::drawChar(char c, uint8_t x, uint8_t page) {
    if (x > WIDTH - 5 || page >= PAGES) return;
    if (c < ' ' || c > '~') c = ' ';

    setPosition(x, page);
    for (uint8_t i = 0; i < 5; i++) {
        sendData(pgm_read_byte(&FONT[c - ' '][i]));
    }
}

void SH1106::drawString(const char* str, uint8_t x, uint8_t page) {
    while (*str) {
        if (x > WIDTH - 5) { // Word wrap
            x = 0;
            page++;
            if (page >= PAGES) return;
        }
        drawChar(*str, x, page);
        x += 6;
        str++;
    }
}