#include <avr/io.h>
#include <util/delay.h>
#include <U8g2lib.h>
#include "I2C_AVR.h"
#include "globals.h"
#include "menu.h"

#define F_CPU 16000000UL
#define MENU_DELAY 1500

U8G2_SSD1363_256X128_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ PD0, /* data=*/ PD1, /* reset=*/ PD3);
Page current_page = MAIN_MENU;
uint8_t selected_index = 0;

int main(void) {
    TWI_init();       // Init I2C
    u8g2.begin();     // Init display

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

        _delay_ms(MENU_DELAY);

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
