#ifndef _SSD1363_H_
#define _SSD1363_H_

#include "DisplayGFX.h"
#include "I2C.h"

#define SSD1363_I2C_ADDRESS 0x3C
#define SSD1363_WIDTH 256
#define SSD1363_HEIGHT 128

#define BUFFER_SIZE ((uint32_t)SSD1363_WIDTH * SSD1363_HEIGHT / 8)

typedef struct {
    DisplayGFX gfx;
    uint8_t buffer[BUFFER_SIZE];

} SSD1363;

void ssd1363_init(SSD1363 *disp);
void ssd1363_draw_pixel(int16_t x, int16_t y, uint16_t color);
void ssd1363_display(SSD1363 *disp);

#endif // _SSD1363_AVR_H_