#include <avr/io.h>
#include <util/delay.h>
#include <U8g2lib.h>
#include "I2C_AVR.h"


#define F_CPU 16000000UL // 16 MHz clock

U8G2_SSD1363_256X128_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ PD0, /* data=*/ PD1 , /* reset=*/ PD3);

int main(void) {

  Serial.begin(115200);
  while (!Serial); // Wait for serial to be ready (optional for some boards)

  // Start I2C with slower clock speed (50 kHz)
  TWI_init();
  // Set up I2C pins (SDA, SCL)


  // Initialize display
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB14_tr); // Set a readable font
    // // Text size 1
    // displaygfx_set_text_color(&disp.gfx, 1); // White
    // displaygfx_set_text_size(&disp.gfx, 1);
    // displaygfx_set_cursor(&disp.gfx, 0, 0);
    // displaygfx_print(&disp.gfx, "Size 1");

    // // Text size 2
    // displaygfx_set_text_size(&disp.gfx, 2);
    // displaygfx_set_cursor(&disp.gfx, 0, 16);
    // displaygfx_print(&disp.gfx, "Size 2");

    // // Text size 3
    // displaygfx_set_text_size(&disp.gfx, 3);
    // displaygfx_set_cursor(&disp.gfx, 0, 48);
    // displaygfx_print(&disp.gfx, "Size 3");

    // Draw a rectangle for context


    while (1) {
        _delay_ms(1000);
    }

    return 0;
}