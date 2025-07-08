// src/display.h (Corrected)

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "I2C.h"
#include <U8g2lib.h> 

typedef enum {
    MAIN_MENU,
    CONTROL_MENU,
    SETTINGS_MENU,
    MODE_CONSTANT_SPEED,
    LINEAR_CONTROL_MODE,
    FLOAT_MODE,
    WARNING_SCREEN,
    CALIBRATION,
    PLACE_WEIGHT,
    DONE_CALIBRATION,
    PRESS_BUTTON
} Page;

// Public function declarations
void display_init(void);
void display_send_buffer(void);
void display_prepare_frame(Page page, uint8_t selected_index);
void display_power_on(void);
void display_power_off(void);


#endif // DISPLAY_H