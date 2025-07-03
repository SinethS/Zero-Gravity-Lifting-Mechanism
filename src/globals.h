#pragma once
#include <U8g2lib.h>

extern U8G2_SSD1363_256X128_F_HW_I2C u8g2;
extern uint8_t selected_index;

enum Page {
  MAIN_MENU,
  CONTROL_MENU,
  SETTINGS_MENU,
  WARNING_SCREEN
};

extern Page current_page;
