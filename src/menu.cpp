// src/menu.cpp (Corrected)

#include "menu.h"

// THE FIX #3: The private enum definition is REMOVED from here.
// static enum Page { ... };  <-- This line is deleted.

// Menu state variables remain private to this file.
static Page current_page = MAIN_MENU;
static uint8_t selected_index = 0;
static uint8_t frame_counter = 0;
static const uint16_t MENU_DELAY_MS = 1500;

void menu_init(void) {
    display_init();
    // This call will now compile correctly.
    display_prepare_frame(current_page, selected_index);
}

void menu_update(void) {
    display_send_buffer();
    _delay_ms(MENU_DELAY_MS);

    // Update state for the next frame.
    frame_counter++;
    switch (frame_counter % 4) {
        case 0: current_page = MAIN_MENU; selected_index = frame_counter % 3; break;
        case 1: current_page = CONTROL_MENU; selected_index = frame_counter % 3; break;
        case 2: current_page = SETTINGS_MENU; selected_index = frame_counter % 3; break;
        case 3: current_page = WARNING_SCREEN; break;
    }

    // Prepare the next frame in the background.
    display_prepare_frame(current_page, selected_index);
}