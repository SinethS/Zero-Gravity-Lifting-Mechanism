#include "DisplayGFX_AVR.h"
#include "font.h"

void displaygfx_init(DisplayGFX *gfx, int16_t w, int16_t h, void (*draw_pixel)(int16_t, int16_t, uint16_t)) {
    gfx->width = w;
    gfx->height = h;
    gfx->cursor_x = 0;
    gfx->cursor_y = 0;
    gfx->text_color = 1; // Default: White
    gfx->text_size = 1; // Default: 1x scale
    gfx->draw_pixel = draw_pixel;
}

void displaygfx_draw_line(DisplayGFX *gfx, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy, e2;

    while (1) {
        if (x0 >= 0 && x0 < gfx->width && y0 >= 0 && y0 < gfx->height)
            gfx->draw_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void displaygfx_draw_rect(DisplayGFX *gfx, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    displaygfx_draw_line(gfx, x, y, x + w - 1, y, color);
    displaygfx_draw_line(gfx, x, y + h - 1, x + w - 1, y + h - 1, color);
    displaygfx_draw_line(gfx, x, y, x, y + h - 1, color);
    displaygfx_draw_line(gfx, x + w - 1, y, x + w - 1, y + h - 1, color);
}

void displaygfx_fill_rect(DisplayGFX *gfx, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (int16_t i = x; i < x + w; i++) {
        for (int16_t j = y; j < y + h; j++) {
            if (i >= 0 && i < gfx->width && j >= 0 && j < gfx->height)
                gfx->draw_pixel(i, j, color);
        }
    }
}

void displaygfx_draw_circle(DisplayGFX *gfx, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r, ddF_x = 1, ddF_y = -2 * r;
    int16_t x = 0, y = r;

    gfx->draw_pixel(x0, y0 + r, color);
    gfx->draw_pixel(x0, y0 - r, color);
    gfx->draw_pixel(x0 + r, y0, color);
    gfx->draw_pixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        gfx->draw_pixel(x0 + x, y0 + y, color);
        gfx->draw_pixel(x0 - x, y0 + y, color);
        gfx->draw_pixel(x0 + x, y0 - y, color);
        gfx->draw_pixel(x0 - x, y0 - y, color);
        gfx->draw_pixel(x0 + y, y0 + x, color);
        gfx->draw_pixel(x0 - y, y0 + x, color);
        gfx->draw_pixel(x0 + y, y0 - x, color);
        gfx->draw_pixel(x0 - y, y0 - x, color);
    }
}

void displaygfx_fill_circle(DisplayGFX *gfx, int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    displaygfx_draw_line(gfx, x0, y0 - r, x0, y0 + r, color);
    // Simplified fill using helper (inlined for brevity)
    int16_t f = 1 - r, ddF_x = 1, ddF_y = -2 * r;
    int16_t x = 0, y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        displaygfx_draw_line(gfx, x0 + x, y0 - y, x0 + x, y0 + y, color);
        displaygfx_draw_line(gfx, x0 - x, y0 - y, x0 - x, y0 + y, color);
        displaygfx_draw_line(gfx, x0 + y, y0 - x, x0 + y, y0 + x, color);
        displaygfx_draw_line(gfx, x0 - y, y0 - x, x0 - y, y0 + x, color);
    }
}

void displaygfx_set_cursor(DisplayGFX *gfx, int16_t x, int16_t y) {
    gfx->cursor_x = x;
    gfx->cursor_y = y;
}

void displaygfx_set_text_color(DisplayGFX *gfx, uint16_t color) {
    gfx->text_color = color;
}

void displaygfx_set_text_size(DisplayGFX *gfx, uint8_t size) {
    gfx->text_size = (size > 0) ? size : 1;
}

void displaygfx_write(DisplayGFX *gfx, uint8_t c) {
    if (c == '\n') {
        gfx->cursor_y += gfx->text_size * 8;
        gfx->cursor_x = 0;
        return;
    }
    if (c < 32 || c > 127) return;

    uint8_t char_idx = c - 32;
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t col = pgm_read_byte(&font[char_idx][i]);
        for (uint8_t j = 0; j < 7; j++) {
            if (col & (1 << j)) {
                for (uint8_t xs = 0; xs < gfx->text_size; xs++) {
                    for (uint8_t ys = 0; ys < gfx->text_size; ys++) {
                        int16_t px = gfx->cursor_x + (i * gfx->text_size) + xs;
                        int16_t py = gfx->cursor_y + (j * gfx->text_size) + ys;
                        if (px >= 0 && px < gfx->width && py >= 0 && py < gfx->height)
                            gfx->draw_pixel(px, py, gfx->text_color);
                    }
                }
            }
        }
    }
    gfx->cursor_x += gfx->text_size * 6;
}

void displaygfx_print(DisplayGFX *gfx, const char *str) {
    while (*str) displaygfx_write(gfx, *str++);
}