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
    displaygfx_fill_circle(&disp.gfx, 64, 0, 20, 1); // Draw a filled circle at (64, 32) with radius 20


   
    // displaygfx_print(&disp.gfx, "Size 1");

    // // Text size 2
    // displaygfx_set_text_size(&disp.gfx, 2);
    // displaygfx_set_cursor(&disp.gfx, 0, 16);
    // displaygfx_print(&disp.gfx, "Size 2");

    // Text size 3
    // displaygfx_set_text_size(&disp.gfx, 3);
    // displaygfx_set_cursor(&disp.gfx, 0, 16);
    // displaygfx_print(&disp.gfx, "Size 3");

    // Update display
    ssd1306_display(&disp);

    // Infinite loop
    while (1) {
        _delay_ms(1000); // Optional delay

        
    }

    return 0;
}