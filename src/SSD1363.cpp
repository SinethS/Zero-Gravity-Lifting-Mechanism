#include "SSD1363.h"

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

    TWI_init();

    // SSD1363 initialization
    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xAE); // Display OFF
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xA8); // Multiplex ratio
    TWI_write(0x7F); // 128 MUX
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xA1); // Segment remap
    TWI_write(0x00); // No remap
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xA2); // Display offset
    TWI_write(0x00); // No offset
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xA4); // Normal display mode
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xA6); // Normal display
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xAB); // VDD regulator
    TWI_write(0x01); // Internal
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xB1); // Phase length
    TWI_write(0x32); // Default
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xB3); // Clock divider
    TWI_write(0xF1); // 80Hz
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xB6); // Second pre-charge
    TWI_write(0x04); // Default
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xBE); // VCOMH
    TWI_write(0x05); // Default
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xBC); // Pre-charge voltage
    TWI_write(0x1F); // Default
    TWI_stop();

    TWI_start();
    TWI_write(SSD1363_I2C_ADDRESS << 1);
    TWI_write(0x00);
    TWI_write(0xAF); // Display ON
    TWI_stop();
  }
