#ifndef _DISPLAY_GFX_AVR_H_
#define _DISPLAY_GFX_AVR_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

typedef struct {
    int16_t width, height;
    int16_t cursor_x, cursor_y;
    uint16_t text_color;
    uint8_t text_size;
    void (*draw_pixel)(int16_t x, int16_t y, uint16_t color); // Function pointer
} DisplayGFX;

void displaygfx_init(DisplayGFX *gfx, int16_t w, int16_t h, void (*draw_pixel)(int16_t, int16_t, uint16_t));
void displaygfx_draw_line(DisplayGFX *gfx, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void displaygfx_draw_rect(DisplayGFX *gfx, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void displaygfx_fill_rect(DisplayGFX *gfx, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void displaygfx_draw_circle(DisplayGFX *gfx, int16_t x0, int16_t y0, int16_t r, uint16_t color);
void displaygfx_fill_circle(DisplayGFX *gfx, int16_t x0, int16_t y0, int16_t r, uint16_t color);
void displaygfx_set_cursor(DisplayGFX *gfx, int16_t x, int16_t y);
void displaygfx_set_text_color(DisplayGFX *gfx, uint16_t color);
void displaygfx_set_text_size(DisplayGFX *gfx, uint8_t size);
void displaygfx_write(DisplayGFX *gfx, uint8_t c);
void displaygfx_print(DisplayGFX *gfx, const char *str);

#endif // _DISPLAY_GFX_AVR_H_