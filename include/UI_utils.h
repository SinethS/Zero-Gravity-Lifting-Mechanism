#include "IO.h"
#include "menu.h"

#ifndef UI_UTILS_H
#define UI_UTILS_H

class UIUtils {

    private:

        IO* io; // Instance of IO class to handle button inputs
        int* button = nullptr; // Pointer to button state

        int prv_press = 0;

        int pressed_button = 0;
        bool menu_update_flag = false; // Flag to indicate if menu needs updating
        


    public:
    // Function to process button presses
    UIUtils(IO *io, int *button);
    void runMenu();

};

#endif // UI_UTILS_H