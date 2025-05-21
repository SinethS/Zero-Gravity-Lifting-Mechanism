#include <avr/io.h>
#include <util/delay.h>
#include <U8g2lib.h>
#include "I2C_AVR.h"

#define F_CPU 16000000UL
#define MENU_DELAY 1500

U8G2_SSD1363_256X128_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ PD0, /* data=*/ PD1 , /* reset=*/ PD3);

// Menu page tracker
enum Page {
  MAIN_MENU,
  CONTROL_MENU,
  SETTINGS_MENU,
  WARNING_SCREEN
};

uint8_t selected_index = 0;
Page current_page = MAIN_MENU;

// Draw a right-facing triangle
void draw_triangle(int x, int y) {
  u8g2.drawPixel(x, y);
  u8g2.drawPixel(x+1, y-1); u8g2.drawPixel(x+1, y+1);
  u8g2.drawPixel(x+2, y-2); u8g2.drawPixel(x+2, y+2);
  u8g2.drawPixel(x+3, y-3); u8g2.drawPixel(x+3, y+3);
  for (int i = -4; i <= 4; i++) {
    u8g2.drawPixel(x+4, y+i);
  }
}

// Generic function to draw a menu
void draw_menu(const char* title, const char* const* items, uint8_t count, uint8_t selected) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);

  // Draw title
  int title_width = u8g2.getStrWidth(title);
  u8g2.drawStr((256 - title_width) / 2, 20, title);

  // Draw options
  for (uint8_t i = 0; i < count; i++) {
    int y = 40 + i * 20;
    if (i == selected) {
      draw_triangle(10, y - 10);
    }
    u8g2.drawStr(30, y, items[i]);
  }

  u8g2.sendBuffer();
}

// Specific menu screens

void show_main_menu(uint8_t selected) {
  const char* options[] = {"Control mode", "Settings", "Exit"};
  draw_menu("Main Menu", options, 3, selected);
}

void show_control_mode_menu(uint8_t selected) {
  const char* options[] = {"Constant speed mode", "Controller mode", "Back"};
  draw_menu("Control mode", options, 3, selected);
}

void show_settings_menu(uint8_t selected) {
  const char* options[] = {"Velocity-", "Acceleration-", "Back"};
  draw_menu("Settings", options, 3, selected);
}

void show_warning_screen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(80, 50, "Warning!");
  u8g2.drawStr(40, 75, "Object too heavy");
  u8g2.sendBuffer();
}

int main(void) {
  // Init I2C and display
  TWI_init();
  u8g2.begin();

  uint32_t last_switch = 0;
  uint8_t frame = 0;

  while (1) {
    switch (current_page) {
      case MAIN_MENU:
        show_main_menu(selected_index);
        break;
      case CONTROL_MENU:
        show_control_mode_menu(selected_index);
        break;
      case SETTINGS_MENU:
        show_settings_menu(selected_index);
        break;
      case WARNING_SCREEN:
        show_warning_screen();
        break;
    }

    // Simulate changing screens (cycling every 1.5 sec for demo)
    _delay_ms(MENU_DELAY);

    // Cycle through pages and selections for demo
    frame++;
    switch (frame % 4) {
      case 0:
        current_page = MAIN_MENU;
        selected_index = frame % 3;
        break;
      case 1:
        current_page = CONTROL_MENU;
        selected_index = frame % 3;
        break;
      case 2:
        current_page = SETTINGS_MENU;
        selected_index = frame % 3;
        break;
      case 3:
        current_page = WARNING_SCREEN;
        break;
    }
  }

  return 0;
}
