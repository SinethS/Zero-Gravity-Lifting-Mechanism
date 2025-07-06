// menu.cpp (Modified)

#include "menu.h"


// --- UPDATE CONSTRUCTOR TO ACCEPT ControllerUtil ---
Menu::Menu(IO* io, int* button, ControllerUtil* controller) 
    : io(io), button(button), controller(controller) {
    // Constructor initializes pointers
}

void Menu::menu_init() {
    display_init();
    display_power_on(); // Ensure display is on at start
    display_on = false;
    // Prepare the initial frame for the first page
    display_prepare_frame(current_page, selected_index);
}


void Menu::menu_process_button(int button_code){
    // If the display is off, any button press will turn it back on.
    if (!display_on && button_code == 4) {
        display_power_on();
        display_on = true;
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
                        display_on = false;
                    }
                    break;
                
                case CONTROL_MENU:
                    if (selected_index == 0) { // "Constant speed mode" selected
                        // --- THIS IS THE KEY TRANSITION ---
                        current_page = MODE_CONSTANT_SPEED;
                        selected_index = 0; // Not used, but reset for consistency
                    } else if (selected_index == 1) { // "Controller mode" selected
                        current_page = LINEAR_CONTROL_MODE;
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



void Menu::menu_update(void) {
    // Don't waste time drawing if the display is off
    if (!display_on) {
        return;
    }
    
    // Prepare the frame buffer based on the current state
    display_prepare_frame(current_page, selected_index);
    // Send the prepared buffer to the physical display
    display_send_buffer();
}

void Menu::runMenu() {
    // Update the menu if the flag is set
    if(*button == -1 && prv_press == 1) {
        pressed_button = 1; // Button pressed
    }else if(*button == -2 && prv_press == 2) {
        pressed_button = 2; // Button pressed
    }else if(*button == -3 && prv_press == 3) {
        pressed_button = 3; // Button pressed
    }else if(*button == -4 && prv_press == 4) {
        pressed_button = 4; // Button pressed
    }
    prv_press = *button; // Update previous button state

    if (pressed_button != 0) {
        // We only care about the press event (positive value)
        if (pressed_button > 0) {
            menu_process_button(pressed_button);
            menu_update_flag = true; // Set flag to update menu
        }
        pressed_button = 0; // Consume the button event to prevent re-triggering
    }

    // MODIFICATION: menu_update now just draws the current state
    if (menu_update_flag) {
        io->detachINTERUPT_PCINT1(); // Detach interrupt to prevent re-entrance
        menu_update(); // Update the menu display
        io->attacthINTERUPT_PCINT1(); // Reattach interrupt for Port K
        menu_update_flag = false; // Reset the flag
    }
}
void Menu::run_active_mode() {
    // If we are not in the constant speed mode, do nothing.
    if (current_page != MODE_CONSTANT_SPEED) {
        return;
    }

    // --- DELEGATE THE WORK ---
    // Call the function from the ControllerUtil instance.
    // It will read the button state directly and control the motor.
    controller->handleButtonControl();
}