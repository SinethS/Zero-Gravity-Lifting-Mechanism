#include "SSD1363_AVR.h"

static SSD1363 *current_disp; // Store current display for draw_pixel callback

void ssd1363_draw_pixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= SSD1363_WIDTH || y < 0 || y >= SSD1363_HEIGHT) return;

    uint16_t byte_idx = (y / 8) * SSD1363_WIDTH + x;
    uint8_t bit = 1 << (y % 8);

    if (color) {
        current_disp->buffer[byte_idx] |= bit;
    } else {
        current_disp->buffer[byte_idx] &= ~bit;
    }
}

void ssd1363_init(SSD1363 *disp) {
    current_disp = disp;
    displaygfx_init(&disp->gfx, SSD1363_WIDTH, SSD1363_HEIGHT, ssd1363_draw_pixel);
    for (uint16_t i = 0; i < SSD1363_WIDTH * SSD1363_HEIGHT / 8; i++) {
        disp->buffer[i] = 0;
    }

    i2c_init();

    // SSD1363 initialization
    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00); // Command
    i2c_write(0xAE); // Display OFF
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xA8); // Multiplex ratio
    i2c_write(0x7F); // 128 MUX
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xA1); // Segment remap
    i2c_write(0x00); // No remap
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xA2); // Display offset
    i2c_write(0x00); // No offset
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xA4); // Normal display mode
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xA6); // Normal display
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xAB); // VDD regulator
    i2c_write(0x01); // Internal
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xB1); // Phase length
    i2c_write(0x32); // Default
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xB3); // Clock divider
    i2c_write(0xF1); // 80Hz
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xB6); // Second pre-charge
    i2c_write(0x04); // Default
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xBE); // VCOMH
    i2c_write(0x05); // Default
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xBC); // Pre-charge voltage
    i2c_write(0x1F); // Default
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0xAF); // Display ON
    i2c_stop();
}

void ssd1363_display(SSD1363 *disp) {
    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0x15); // Column address
    i2c_write(0x00); // Start
    i2c_write(0xFF); // End
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00);
    i2c_write(0x75); // Row address
    i2c_write(0x00); // Start
    i2c_write(0x7F); // End
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x40); // Data mode
    for (uint16_t i = 0; i < SSD1363_WIDTH * SSD1363_HEIGHT / 8; i++) {
        i2c_write(disp->buffer[i]);
    }
    i2c_stop();
}