#include "SSD1363_AVR.h"
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL // 16 MHz clock

int main(void) {
    SSD1363 disp;

    ssd1363_init(&disp);

    // Text size 1
    displaygfx_set_text_color(&disp.gfx, 1); // White
    displaygfx_set_text_size(&disp.gfx, 1);
    displaygfx_set_cursor(&disp.gfx, 0, 0);
    displaygfx_print(&disp.gfx, "Size 1");

    // Text size 2
    displaygfx_set_text_size(&disp.gfx, 2);
    displaygfx_set_cursor(&disp.gfx, 0, 16);
    displaygfx_print(&disp.gfx, "Size 2");

    // Text size 3
    displaygfx_set_text_size(&disp.gfx, 3);
    displaygfx_set_cursor(&disp.gfx, 0, 48);
    displaygfx_print(&disp.gfx, "Size 3");

    // Draw a rectangle for context
    //displaygfx_draw_rect(&disp.gfx, 10, 10, 50, 30, 1);

    ssd1363_display(&disp);

    while (1) {
        _delay_ms(1000);
    }

    return 0;
}