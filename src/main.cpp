#include <avr/io.h>
#include <util/delay.h>
#include <U8g2lib.h>
#include "I2C_AVR.h"


#define F_CPU 16000000UL // 16 MHz clock
#define MENU_DELAY 1500

U8G2_SSD1363_256X128_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ PD0, /* data=*/ PD1) ;
// enum Page {
//   MAIN_MENU,
//   CONTROL_MENU,
//   SETTINGS_MENU,
//   WARNING_SCREEN
// };

// uint8_t selected_index = 0;
// Page current_page = MAIN_MENU;

// // Draw a right-facing triangle
// void draw_triangle(int x, int y) {
//   u8g2.drawPixel(x, y);
//   u8g2.drawPixel(x+1, y-1); u8g2.drawPixel(x+1, y+1);
//   u8g2.drawPixel(x+2, y-2); u8g2.drawPixel(x+2, y+2);
//   u8g2.drawPixel(x+3, y-3); u8g2.drawPixel(x+3, y+3);
//   for (int i = -4; i <= 4; i++) {
//     u8g2.drawPixel(x+4, y+i);
//   }
// }

// // Generic function to draw a menu
// void draw_menu(const char* title, const char* const* items, uint8_t count, uint8_t selected) {
//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB14_tr);

//   // Draw title
//   int title_width = u8g2.getStrWidth(title);
//   u8g2.drawStr((256 - title_width) / 2, 20, title);

//   // Draw options
//   for (uint8_t i = 0; i < count; i++) {
//     int y = 40 + i * 20;
//     if (i == selected) {
//       draw_triangle(10, y - 10);
//     }
//     u8g2.drawStr(30, y, items[i]);
//   }

//   u8g2.sendBuffer();
// }

// // Specific menu screens

// void show_main_menu(uint8_t selected) {
//   const char* options[] = {"Control mode", "Settings", "Exit"};
//   draw_menu("Main Menu", options, 3, selected);
// }

// void show_control_mode_menu(uint8_t selected) {
//   const char* options[] = {"Constant speed mode", "Controller mode", "Back"};
//   draw_menu("Control mode", options, 3, selected);
// }

// void show_settings_menu(uint8_t selected) {
//   const char* options[] = {"Velocity-", "Acceleration-", "Back"};
//   draw_menu("Settings", options, 3, selected);
// }

// void show_warning_screen() {
//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB14_tr);
//   u8g2.drawStr(80, 50, "Warning!");
//   u8g2.drawStr(40, 75, "Object too heavy");
//   u8g2.sendBuffer();
// }

int main(void) {

  Serial.begin(115200);
  while (!Serial); // Wait for serial to be ready (optional for some boards)

  // Start I2C with slower clock speed (50 kHz)
  TWI_init();
  // Set up I2C pins (SDA, SCL)


  // Initialize display
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB14_tr); // Set a readable font

  // uint32_t last_switch = 0;
  // uint8_t frame = 0;
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
      // Init I2C and display
      for (int i = 1; i <= 50; i++) {

        u8g2.clearBuffer();

        // Convert number to string
        char buffer[10];
        sprintf(buffer, "%d", i);

        // Calculate text width and position
        int text_width = u8g2.getStrWidth(buffer);
        int x = (256 - text_width) / 2;
        int y = (128 + 14) / 2; // Center vertically

        // Draw and display
        u8g2.drawStr(x, y, buffer);
        u8g2.sendBuffer();
      }

  }

  return 0;
}