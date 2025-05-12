#include "SSD1363_AVR.h"
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL // 16 MHz clock

int main(void) {
    SSD1363 disp;

    // Initialize display
    ssd1306_init(&disp);

    // Configure text settings
    displaygfx_set_text_color(&disp.gfx, 1); // White
    displaygfx_set_text_size(&disp.gfx, 1);
    displaygfx_set_cursor(&disp.gfx, 0, 0);
    displaygfx_print(&disp.gfx, "Hello, SSD1363!");

    // Draw graphics
    displaygfx_draw_rect(&disp.gfx, 10, 10, 50, 30, 1);
    displaygfx_fill_circle(&disp.gfx, 100, 100, 20, 1);

    // Update display
    ssd1306_display(&disp);

    // Infinite loop
    while (1) {
        _delay_ms(1000); // Optional delay

        
    }

    return 0;
}