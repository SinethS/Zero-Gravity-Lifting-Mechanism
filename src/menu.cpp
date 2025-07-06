// menu.cpp (Modified)

#include "menu.h"

// Menu state variables remain private to this file
static Page current_page = MAIN_MENU;
static uint8_t selected_index = 0;
static bool display_is_on = true;

// Helper array to know how many selectable items are on each page.
// The order MUST match the Page enum in display.h
static const uint8_t page_item_counts[] = {
    3, // MAIN_MENU
    3, // CONTROL_MENU
    3, // SETTINGS_MENU
    0,  // WARNING_SCREEN (no selectable items)
    0,  // MODE_CONSTANT_SPEED (no selectable items)
    0   // LINEAR_CONTROL (no selectable items)
};


void menu_init(void) {
    display_init();
    display_power_on(); // Ensure display is on at start
    display_is_on = true;
    // Prepare the initial frame for the first page
    display_prepare_frame(current_page, selected_index);
}

// Reworked function to handle all button inputs and change the menu state
void menu_process_button(int button_code) {
    // If the display is off, any button press will turn it back on.
    if (!display_is_on) {
        display_power_on();
        display_is_on = true;
        // After turning on, we don't process the button further to avoid
        // an immediate action. The next button press will be registered.
        return;
    }

    // Process buttons based on their code from IO.cpp
    // 1: Select (PJ0), 2: Down (PJ1), 3: Up (PJ2), 4: Menu/Back (PJ3)
    switch (button_code) {
        case 1: // --- SELECT ---
            switch (current_page) {
                case MAIN_MENU:
                    if (selected_index == 0) { // "Control mode" selected
                        current_page = CONTROL_MENU;
                        selected_index = 0;
                    } else if (selected_index == 1) { // "Settings" selected
                        current_page = SETTINGS_MENU;
                        selected_index = 0;
                    } else if (selected_index == 2) { // "Exit" selected
                        display_power_off();
                        display_is_on = false;
                    }
                    break;
                
                case CONTROL_MENU:
                    if (selected_index == 0) { // "Constant speed mode" selected
                        // --- THIS IS THE KEY TRANSITION ---
                        current_page = MODE_CONSTANT_SPEED;
                        selected_index = 0; // Not used, but reset for consistency
                    } else if (selected_index == 1) { // "Controller mode" selected
                        current_page = LINEAR_CONTROL;
                    } else if (selected_index == 2) { // "Back" selected
                        current_page = MAIN_MENU;
                        selected_index = 0;
                    }
                    break;

                case SETTINGS_MENU:
                    if (selected_index == 2) { // "Back" selected
                        current_page = MAIN_MENU;
                        selected_index = 0;
                    }
                    // TODO: Add actions for "Velocity" and "Acceleration"
                    break;

                case WARNING_SCREEN:
                    // Any action on warning screen returns to main menu
                    current_page = MAIN_MENU;
                    selected_index = 0;
                    break;

                // case MODE_CONSTANT_SPEED:
                //     // In constant speed mode, we don't change pages,
                //     // but we can handle the button press to control the motor.
                //     // This will be handled in run_active_mode().
                //     run_active_mode();
                //     break; 
            }
            break;

        case 2: // --- DOWN ---
            // Only act if the current page has selectable items
            if (page_item_counts[current_page] > 0) {
                selected_index++;
                // Wrap around if we go past the last item
                if (selected_index >= page_item_counts[current_page]) {
                    selected_index = 0;
                }
            }
            break;

        case 3: // --- UP ---
            // Only act if the current page has selectable items
            if (page_item_counts[current_page] > 0) {
                // Wrap around if we go before the first item
                if (selected_index == 0) {
                    selected_index = page_item_counts[current_page] - 1;
                } else {
                    selected_index--;
                }
            }
            break;

        case 4: // --- BACK / MENU ---
            // If we are in a sub-menu, go back to the main menu.
            // If on the warning screen, also go back.
            if (current_page != MAIN_MENU) {
                current_page = MAIN_MENU;
                selected_index = 0;
            }
            break;
    }
}

// Reworked update function. It's now non-blocking.
void menu_update(void) {
    // Don't waste time drawing if the display is off
    if (!display_is_on) {
        return;
    }
    
    // Prepare the frame buffer based on the current state
    display_prepare_frame(current_page, selected_index);
    // Send the prepared buffer to the physical display
    display_send_buffer();
}