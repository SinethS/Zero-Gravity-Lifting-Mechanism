// menu.cpp

#include "menu.h"

// Constructor definition
Menu::Menu(IO* io, int* button, ControllerUtil* controller) 
    : io(io), button(button), controller(controller) {}

void Menu::menu_init() {
    display_init();
    display_power_off();
    display_on = false;
    menu_update_flag = false;
}

void Menu::menu_process_button(int button_code) {
    if (!display_on && button_code == 1) {
        display_power_on();
        display_on = true;
        menu_update_flag = false;
        return;
    }

    if (current_page == MODE_CONSTANT_SPEED || current_page == LINEAR_CONTROL_MODE) {
        if (button_code == 1) { // BACK
            current_page = CONTROL_MENU;
            selected_index = 0;
            controller->handleFloatControl(); // Stop the active mode
        }
        return;
    }
    process_menu_navigation(button_code);
}

void Menu::process_menu_navigation(int button_code) {
    switch (button_code) {
        case 4: // SELECT
            switch (current_page) {
                case MAIN_MENU:
                    if (selected_index == 0) { current_page = CONTROL_MENU; selected_index = 0; }
                    else if (selected_index == 1) { current_page = SETTINGS_MENU; selected_index = 0; }
                    else if (selected_index == 2) { display_power_off(); display_on = false; }
                    break;
                case CONTROL_MENU:
                    if (selected_index == 0) { current_page = MODE_CONSTANT_SPEED; selected_index = 0; }
                    else if (selected_index == 1) { current_page = LINEAR_CONTROL_MODE; selected_index = 0; }
                    else if (selected_index == 2) { current_page = FLOAT_MODE; selected_index = 0; }
                    else if (selected_index == 3) { current_page = MAIN_MENU; selected_index = 0; }
                    break;
                case SETTINGS_MENU:
                    if (selected_index == 2) { current_page = MAIN_MENU; selected_index = 0; }
                    break;
                case MODE_CONSTANT_SPEED:
                    
                    break;
                case LINEAR_CONTROL_MODE:
 
                    break;
                case FLOAT_MODE:
                    
                    break;
                case WARNING_SCREEN:
                    
                    break;
                case CALIBRATION:
                    
                    break;
                case PLACE_WEIGHT:
                    
                    break;
                case DONE_CALIBRATION:
                    
                    break;
                case PRESS_BUTTON:

                    break;
                case PLEASE_WAIT:
                    break;

                case REMOVE_WEIGHT:
                    break;

            }
            break;
        case 3: // DOWN
            if (page_item_counts[current_page] > 0) {
                selected_index = (selected_index + 1) % page_item_counts[current_page];
            }
            break;
        case 2: // UP
            if (page_item_counts[current_page] > 0) {
                selected_index = (selected_index == 0) ? (page_item_counts[current_page] - 1) : (selected_index - 1);
            }
            break;
        case 1: // BACK
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

    if ((pressed_button_code > 0) && (((pressed_button_code != 2 || pressed_button_code != 3) && (current_page != MODE_CONSTANT_SPEED )) || (current_page == MODE_CONSTANT_SPEED && pressed_button_code == 1))) {
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
            controller->touchflag = true; // Reset touch flag for next iteration
            controller->handleButtonControl();
            break; // 'break' is essential to exit the switch

        case LINEAR_CONTROL_MODE:
            // When in this mode, only run the linear (potentiometer) control logic.
            controller->touchflag = true; // Reset touch flag for next iteration
            controller->handlLinearControl();
            break;
        case FLOAT_MODE:
            // When in this mode, only run the float control logic.
            controller->touchflag = true; // Reset touch flag for next iteration
            controller->handleFloatControl();
            break;

    }
}

// to display calibration, weight add and warning screen
void Menu::showWarningScreen() {
    display_power_on();
    display_prepare_frame(WARNING_SCREEN, 0);
    display_send_buffer();
}

void Menu::showPlaceWeightScreen() {
    display_power_on();
    display_prepare_frame(PLACE_WEIGHT, 0);
    display_send_buffer();
}

void Menu::showCalibrationScreen() {
    display_power_on();
    display_prepare_frame(CALIBRATION, 0);
    display_send_buffer();
}

void Menu::showDoneCalibrationScreen(){
    display_power_on();
    display_prepare_frame(DONE_CALIBRATION, 0);
    display_send_buffer();
}

void Menu::showPressButtonScreen() {
    display_power_on();
    display_prepare_frame(PRESS_BUTTON, 0);
    display_send_buffer();

}

void Menu::showPleaseWaitScreen() {
    display_power_on();
    display_prepare_frame(PLEASE_WAIT, 0);
    display_send_buffer();
}

void Menu::removeWeightScreen() {
    display_power_on();
    display_prepare_frame(REMOVE_WEIGHT, 0);
    display_send_buffer();
}

void Menu::zeroGravityMode(){
    if (!display_on && *button == -2) {

        controller->zeroGravity(); // Call zero gravity mode function
    } else {
        return;
    }
}

void Menu::set_display_power_off() {
    io->controlLEDs(0b1000, true); // Turn off all LEDs
    display_power_off(); // Turn off the display
    display_on = false;
    _delay_ms(500); // Wait for 0.5 seconds
    io->controlLEDs(0b0000, true); // Turn off all LEDs
}