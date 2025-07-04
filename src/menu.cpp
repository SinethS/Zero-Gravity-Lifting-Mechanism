// menu.cpp (Modified)

#include "menu.h"


// Menu state variables remain private to this file
static Page current_page = MAIN_MENU;
static uint8_t selected_index = 0;

void menu_init(void) {
    display_init();
    // Prepare the initial frame for the first page
    display_prepare_frame(current_page, selected_index);
}

// New function to handle button inputs and change the menu state
void menu_process_button(int button_code) {
    // We only care about button 1 press (positive value) for page navigation
    if (button_code == 1) {
        // Cycle to the next page
        switch (current_page) {
            case MAIN_MENU:      current_page = CONTROL_MENU;  break;
            case CONTROL_MENU:   current_page = SETTINGS_MENU; break;
            case SETTINGS_MENU:  current_page = WARNING_SCREEN;break;
            case WARNING_SCREEN: current_page = MAIN_MENU;     break;
        }
        // Reset the selected item index when changing pages
        selected_index = 0;
    }
    // TODO: Add logic for other buttons (e.g., button 2 to cycle selected_index)
}

// Reworked update function. It's now non-blocking.
// It prepares and sends the frame for the *current* state.
void menu_update(void) {
    // Prepare the frame buffer based on the current state
    display_prepare_frame(current_page, selected_index);
    // Send the prepared buffer to the physical display
    display_send_buffer();
}