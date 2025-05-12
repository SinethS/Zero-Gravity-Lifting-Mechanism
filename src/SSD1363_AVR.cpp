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

void ssd1306_init(SSD1363 *disp) {
    current_disp = disp;
    displaygfx_init(&disp->gfx, SSD1363_WIDTH, SSD1363_HEIGHT, ssd1363_draw_pixel);
    for (uint16_t i = 0; i < SSD1363_WIDTH * SSD1363_HEIGHT / 8; i++) {
        disp->buffer[i] = 0;
    }

    i2c_init();

    // SSD1306 Initialization
    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1);
    i2c_write(0x00); // Command mode

    i2c_write(0xAE); // Display OFF
    i2c_write(0xD5); // Set display clock divide ratio/oscillator frequency
    i2c_write(0x80); // Default
    i2c_write(0xA8); // Set multiplex ratio
    i2c_write(0x3F); // 1/64 duty (128 pixels height)
    i2c_write(0xD3); // Set display offset
    i2c_write(0x00); // No offset
    i2c_write(0x40); // Set start line at 0
    i2c_write(0x8D); // Enable charge pump
    i2c_write(0x14); // Enable
    i2c_write(0x20); // Set memory addressing mode
    i2c_write(0x00); // Horizontal addressing mode
    i2c_write(0xA1); // Set segment re-map (column address 127 is mapped to SEG0)
    i2c_write(0xC8); // Set COM output scan direction (remapped mode)
    i2c_write(0xDA); // Set COM pins hardware configuration
    i2c_write(0x12); //
    i2c_write(0x81); // Set contrast control
    i2c_write(0x7F); //
    i2c_write(0xD9); // Set pre-charge period
    i2c_write(0xF1); //
    i2c_write(0xDB); // Set VCOMH deselect level
    i2c_write(0x40); //
    i2c_write(0xA4); // Entire display ON (resume to RAM)
    i2c_write(0xA6); // Normal display (not inverted)
    i2c_write(0xAF); // Display ON

    i2c_stop();
}


void ssd1306_display(SSD1363 *disp) {
    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1); // or SSD1306_I2C_ADDRESS
    i2c_write(0x00); // Command mode
    i2c_write(0x21); // Set column address
    i2c_write(0x00); // Column start
    i2c_write(0x7F); // Column end (127)
    i2c_write(0x22); // Set page address
    i2c_write(0x00); // Page start
    i2c_write(0x07); // Page end (for 64px height: pages 0–7)
    i2c_stop();

    i2c_start();
    i2c_write(SSD1363_I2C_ADDRESS << 1); // or SSD1306_I2C_ADDRESS
    i2c_write(0x40); // Data mode

    for (uint16_t i = 0; i < SSD1363_WIDTH * SSD1363_HEIGHT / 8; i++) {
        i2c_write(disp->buffer[i]);
    }

    i2c_stop();
}

