// src/display.h (Corrected)

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "I2C.h"
#include <U8g2lib.h> 

// THE FIX #1: The Page enum is now public here.
// Any file that includes display.h will know what a "Page" is.
enum Page {
    MAIN_MENU,
    CONTROL_MENU,
    SETTINGS_MENU,
    WARNING_SCREEN
};

// Initializes the display hardware.
void display_init(void);

// Sends the internal buffer to the physical display.
void display_send_buffer(void);

// THE FIX #2: The function prototype is now declared publicly.
// This makes it visible to menu.cpp and other files.
void display_prepare_frame(Page page, uint8_t selected_index);

#endif // DISPLAY_H