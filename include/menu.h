// menu.h

#ifndef MENU_H
#define MENU_H

#include "display.h" 
#include "controller_utils.h"
#include "IO.h"// Include for the Page enum definition

class ControllerUtil;
class Menu{
    private:
        Page current_page = MAIN_MENU; // Current page of the menu
        uint8_t selected_index = 0; // Currently selected item index
        IO* io; // Instance of IO class to handle button inputs
        int* button = nullptr; // Pointer to button state
        ControllerUtil* controller; // Pointer to ControllerUtil for handling controller inputs
        bool display_on = false; // Flag to indicate if the display is on

        int prv_press = 0;

        int pressed_button = 0;
        bool menu_update_flag = false; // Flag to indicate if menu needs updating
        // Helper array to know how many selectable items are on each page.
        const int page_item_counts[13] = {
            3, // MAIN_MENU
            4, // CONTROL_MENU
            3, // SETTINGS_MENU
            0, // MODE_CONSTANT_SPEED (no selectable items, it's an active mode)
            0, // LINEAR_CONTROL_MODE (no selectable items, it's an active mode)
            0, // FLOAT_MODE (no selectable items, it's an active mode)
            0,  // WARNING_SCREEN (no selectable items, it's a warning screen)
            0,  // CALIBRATION (no selectable items, it's a calibration screen)
            0,  // PLACE_WEIGHT (no selectable items, it's a place weight screen)
            0,  // DONE_CALIBRATION (no selectable items, it's a place weight screen)
            0, // PRESS_BUTTON (no selectable items, it's a press button screen)
            0, // PLEASE_WAIT (no selectable items, it's a please wait screen)
            0 // REMOVE_WEIGHT(no selectable items, it's a please wait screen)
        }; // Number of selectable items on each page

    public:
        Menu(IO* io, int* button, ControllerUtil* controller);
        void menu_init(); // Initialize the menu
        void menu_update(); // Update the menu display
        void menu_process_button(int button_code); // Process button inputs
        void runMenu();
        void run_active_mode(); // Run the active mode (e.g., constant speed mode)
        void process_menu_navigation(int button_code); // Process menu navigation based on button inputs
        void showWarningScreen(); // Display warning screen
        void showPlaceWeightScreen(); // Display screen to place weight
        void showCalibrationScreen(); // Display calibration screen
        void showDoneCalibrationScreen();
        void showPressButtonScreen(); // Display screen to press button
        void showPleaseWaitScreen(); // Display screen to please wait
        void removeWeightScreen(); // Display screen to remove weight
        void zeroGravityMode(); // Zero gravity mode for touch controller
        void set_display_power_off();
        
}; // Forward declaration


#endif // MENU_H