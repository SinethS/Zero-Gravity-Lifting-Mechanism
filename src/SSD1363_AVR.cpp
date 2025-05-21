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

void ssd1363_send_command(uint8_t cmd) {
    i2c_start();
    i2c_write((SSD1363_I2C_ADDRESS << 1) | 0); // Write mode
    i2c_write(0x00); // Control byte: command
    i2c_write(cmd);
    i2c_stop();
}

void ssd1363_send_command_2(uint8_t cmd, uint8_t arg) {
    i2c_start();
    i2c_write((SSD1363_I2C_ADDRESS << 1) | 0);
    i2c_write(0x00);
    i2c_write(cmd);
    i2c_write(arg);
    i2c_stop();
}

void ssd1363_init(SSD1363 *disp) {
    current_disp = disp;
    displaygfx_init(&disp->gfx, SSD1363_WIDTH, SSD1363_HEIGHT, ssd1363_draw_pixel);

    // Clear buffer
    for (uint16_t i = 0; i < (SSD1363_WIDTH * SSD1363_HEIGHT / 8); i++) {
        disp->buffer[i] = 0;
    }

    // Reset sequence
    SSD1363_RST_DDR |= (1 << SSD1363_RST_PIN);
    SSD1363_RST_PORT &= ~(1 << SSD1363_RST_PIN);
    _delay_ms(10);
    SSD1363_RST_PORT |= (1 << SSD1363_RST_PIN);
    _delay_ms(10);

    // Initialize I2C bus
    i2c_init();

    // SSD1363 command init sequence
    ssd1363_send_command(0xAE);                  // Display OFF
    ssd1363_send_command(0x40);                  // Set Display Start Line
    ssd1363_send_command_2(0x20, 0x00);          // Horizontal addressing mode
    ssd1363_send_command_2(0xA8, 0x3F);          // Multiplex ratio (64-1 = 0x3F)
    ssd1363_send_command_2(0xA1, 0x00);          // Segment remap (0x00 = column 0)
    ssd1363_send_command_2(0xA2, 0x00);          // Display offset (0 = no offset)
    ssd1363_send_command(0xA4);                  // Resume to RAM content
    ssd1363_send_command(0xA6);                  // Normal display (not inverted)
    ssd1363_send_command_2(0xAB, 0x01);          // Internal VDD regulator
    ssd1363_send_command_2(0xB1, 0x32);          // Phase length
    ssd1363_send_command_2(0xB3, 0xF1);          // Clock divider
    ssd1363_send_command_2(0xB6, 0x04);          // Second precharge
    ssd1363_send_command_2(0xBE, 0x05);          // VCOMH
    ssd1363_send_command_2(0xBC, 0x1F);          // Pre-charge voltage
    ssd1363_send_command(0xAF);                  // Display ON
}

void ssd1363_display(SSD1363 *disp) {
    // Set column address (0-127)
    ssd1363_send_command_2(0x15, 0x00); // Start column = 0
    ssd1363_send_command_2(0x15, 0x7F); // End column = 127

    // Set row address (0-63)
    ssd1363_send_command_2(0x75, 0x00); // Start row = 0
    ssd1363_send_command_2(0x75, 0x3F); // End row = 63

    // Send buffer data in larger chunks (32 bytes)
    const uint16_t total = SSD1363_WIDTH * SSD1363_HEIGHT / 8;
    const uint16_t chunkSize = 32;

    for (uint16_t i = 0; i < total; i += chunkSize) {
        i2c_start();
        i2c_write((SSD1363_I2C_ADDRESS << 1) | 0);
        i2c_write(0x40); // Data mode
        for (uint16_t j = 0; j < chunkSize && (i + j) < total; j++) {
            i2c_write(disp->buffer[i + j]);
        }
        i2c_stop();
    }
}