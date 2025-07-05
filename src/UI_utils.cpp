#include "UI_utils.h"


UIUtils::UIUtils(IO *io, int *button) : io(io), button(button) {

}

void UIUtils::runMenu() {
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