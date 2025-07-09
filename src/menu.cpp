// menu.cpp

#include "menu.h"

// Constructor definition
Menu::Menu(IO* io, int* button, ControllerUtil* controller, motor *stepper) 
    : io(io), button(button), controller(controller), stepper(stepper) {}

void Menu::menu_init() {
    display_init();
    display_power_off();
    display_on = false;
    menu_update_flag = false;
}

void Menu::menu_process_button(int button_code) {
    if (!display_on && button_code == 4) {
        display_power_on();
        display_on = true;
        menu_update_flag = false;
        return;
    }

    if (current_page == MODE_CONSTANT_SPEED || current_page == LINEAR_CONTROL_MODE) {
        if (button_code == 4) { // BACK
            current_page = CONTROL_MENU;
            selected_index = 0;
            stepper->stopMotor();
            
        }
        return;
    }
    process_menu_navigation(button_code);
}

void Menu::process_menu_navigation(int button_code) {
    switch (button_code) {
        case 1: // SELECT
            switch (current_page) {
                case MAIN_MENU:
                    if (selected_index == 0) { current_page = CONTROL_MENU; selected_index = 0; }
                    else if (selected_index == 1) { current_page = SETTINGS_MENU; selected_index = 0; }
                    else if (selected_index == 2) { display_power_off(); display_on = false; }
                    break;
                case CONTROL_MENU:
                    if (selected_index == 0) { current_page = MODE_CONSTANT_SPEED; selected_index = 0; }
                    else if (selected_index == 1) { current_page = LINEAR_CONTROL_MODE; selected_index = 0; }
                    else if (selected_index == 2) { current_page = MAIN_MENU; selected_index = 0; }
                    break;
                case SETTINGS_MENU:
                    if (selected_index == 2) { current_page = MAIN_MENU; selected_index = 0; }
                    break;
                case WARNING_SCREEN:
                    
                    break;
                case MODE_CONSTANT_SPEED:
                    
                    break;
                case LINEAR_CONTROL_MODE:
 
                    break;
            }
            break;
        case 2: // DOWN
            if (page_item_counts[current_page] > 0) {
                selected_index = (selected_index + 1) % page_item_counts[current_page];
            }
            break;
        case 3: // UP
            if (page_item_counts[current_page] > 0) {
                selected_index = (selected_index == 0) ? (page_item_counts[current_page] - 1) : (selected_index - 1);
            }
            break;
        case 4: // BACK
            if (current_page != MAIN_MENU) {
                current_page = MAIN_MENU;
                selected_index = 0;
            }
            break;
    }
}

void Menu::menu_update(void) {
    if (!display_on) return;
    display_prepare_frame(current_page, selected_index);
    display_send_buffer();
}

void Menu::runMenu() {
    int current_button_state = *button;
    int pressed_button_code = 0;
    if (current_button_state > 0 && prv_press <= 0) {
        pressed_button_code = current_button_state;
    }
    prv_press = current_button_state;

    if (pressed_button_code > 0 && (( (pressed_button_code != 2 || pressed_button_code != 3) && current_page != MODE_CONSTANT_SPEED || current_page == MODE_CONSTANT_SPEED && pressed_button_code == 4))) {
        menu_process_button(pressed_button_code);
        menu_update_flag = true;
    }
    if (menu_update_flag) {
        io->detachINTERUPT_PCINT1();
        menu_update();
        io->attacthINTERUPT_PCINT1();
        menu_update_flag = false;
    }
}

void Menu::run_active_mode() {
    // Dispatch to the correct control function based on the current page/mode.
    switch (current_page) {
        case MODE_CONSTANT_SPEED:
            // When in this mode, only run the button control logic.
            controller->handleButtonControl();
            break; // 'break' is essential to exit the switch

        case LINEAR_CONTROL_MODE:
            // When in this mode, only run the linear (potentiometer) control logic.
            controller->handlLinearControl();
            break;
        default:
            // If not in an active mode, do nothing or handle other cases if needed.
            break;
    }
}