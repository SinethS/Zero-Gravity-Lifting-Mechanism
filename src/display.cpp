// src/display.cpp (Corrected and Refactored)

#include "display.h"

static u8g2_t u8g2;

// This helper function remains the same
static void draw_triangle(int x, int y) {
    u8g2_DrawPixel(&u8g2, x, y);
    u8g2_DrawPixel(&u8g2, x-1, y-1); u8g2_DrawPixel(&u8g2, x-1, y+1);
    u8g2_DrawPixel(&u8g2, x-2, y-2); u8g2_DrawPixel(&u8g2, x-2, y+2);
    u8g2_DrawPixel(&u8g2, x-3, y-3); u8g2_DrawPixel(&u8g2, x-3, y+3);
    for (int i = -4; i <= 4; i++) {
        u8g2_DrawPixel(&u8g2, x-4, y+i);
    }
}

// Draw an upward pointing triangle
static void draw_triangle_up(int x, int y) {
    u8g2_DrawPixel(&u8g2, x, y);
    u8g2_DrawPixel(&u8g2, x-1, y+1); u8g2_DrawPixel(&u8g2, x+1, y+1);
    u8g2_DrawPixel(&u8g2, x-2, y+2); u8g2_DrawPixel(&u8g2, x+2, y+2);
}

// Draw a downward pointing triangle
static void draw_triangle_down(int x, int y) {
    u8g2_DrawPixel(&u8g2, x, y);
    u8g2_DrawPixel(&u8g2, x-1, y-1); u8g2_DrawPixel(&u8g2, x+1, y-1);
    u8g2_DrawPixel(&u8g2, x-2, y-2); u8g2_DrawPixel(&u8g2, x+2, y-2);
}

// Draw a left-pointing triangle
static void draw_triangle_left(int x, int y) {
    u8g2_DrawPixel(&u8g2, x, y);
    u8g2_DrawPixel(&u8g2, x+1, y-1); u8g2_DrawPixel(&u8g2, x+1, y+1);
    u8g2_DrawPixel(&u8g2, x+2, y-2); u8g2_DrawPixel(&u8g2, x+2, y+2);
}

// Draw a right-pointing triangle
static void draw_triangle_right(int x, int y) {
    u8g2_DrawPixel(&u8g2, x, y);
    u8g2_DrawPixel(&u8g2, x-1, y-1); u8g2_DrawPixel(&u8g2, x-1, y+1);
    u8g2_DrawPixel(&u8g2, x-2, y-2); u8g2_DrawPixel(&u8g2, x-2, y+2);
}



// This helper function remains the same
static void draw_menu_content(const char* title, const char* const* items, uint8_t count, uint8_t selected) {
    int title_width = u8g2_GetStrWidth(&u8g2, title);
    u8g2_DrawStr(&u8g2, (256 - title_width) / 2, 20, title);
    for (uint8_t i = 0; i < count; i++) {
        int y_pos = 40 + i * 20;
        if (i == selected) {
            draw_triangle(10, y_pos - 10);
        }
        u8g2_DrawStr(&u8g2, 30, y_pos, items[i]);
    }
}

void display_init(void) {
    i2c_avr_init(); // Initialize I2C for U8g2
    u8g2_Setup_ssd1363_i2c_256x128_f(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_gpio_and_delay_avr);
    u8g2_InitDisplay(&u8g2);
    // The display is turned on by default
    u8g2_SetPowerSave(&u8g2, 0);
}

void display_send_buffer(void) {
    u8g2_SendBuffer(&u8g2);
}

// NEW FUNCTION: Turns the display ON
void display_power_on(void) {
    u8g2_SetPowerSave(&u8g2, 0);
}

// NEW FUNCTION: Turns the display OFF (into power save mode)
void display_power_off(void) {
    u8g2_SetPowerSave(&u8g2, 1);
}

// This function remains the same
void display_prepare_frame(Page page, uint8_t selected_index) {
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);

    switch (page) {
        case MAIN_MENU: {
            const char* options[] = {"Control mode", "Settings", "Exit"};
            draw_menu_content("Main Menu", options, 3, selected_index);
            break;
        }
        case CONTROL_MENU: {
            const char* options[] = {"Constant speed mode", "Controller mode", "Float mode", "Back"};
            draw_menu_content("Control mode", options, 4, selected_index);
            break;
        }
        //case SETTINGS_MENU: {
            //const char* options[] = {"Velocity-", "Acceleration-", "Back"};
            //draw_menu_content("Settings", options, 3, selected_index);
            //break;
        //}

        case SETTINGS_MENU: {
            const char* options[] = {"Velocity-", "Acceleration-", "Back"};
            draw_menu_content("Settings", options, 3, selected_index);

            // Draw arrow and value for Velocity-
            // draw_triangle(180, 30);                // Arrow next to Velocity-
            u8g2_DrawStr(&u8g2, 190, 35, "11");    // Display value "11"

            // Draw arrow and value for Acceleration-
            // draw_triangle(180, 50);                // Arrow next to Acceleration-
            u8g2_DrawStr(&u8g2, 190, 55, "11");    // Display value "11"

            break;
        }

        case WARNING_SCREEN: {
            u8g2_DrawStr(&u8g2, 80, 50, "Warning!");
            u8g2_DrawStr(&u8g2, 40, 75, "Object too heavy");
            break;
        }
        case MODE_CONSTANT_SPEED: {
            u8g2_DrawStr(&u8g2, 50, 20, "Constant Speed");
            u8g2_DrawStr(&u8g2, 20, 110, "Press back to exit.");

            // Draw UP label with triangle
            u8g2_DrawStr(&u8g2, 50, 50, "UP");
            draw_triangle_up(40, 43); // Upward triangle next to UP

            // Draw DOWN label with triangle
            u8g2_DrawStr(&u8g2, 50, 70, "DOWN");
            draw_triangle_down(40, 64); // Downward triangle next to DOWN

            break;
        }
        case LINEAR_CONTROL_MODE: {
            u8g2_DrawStr(&u8g2, 50, 20, "Linear Control");
            u8g2_DrawStr(&u8g2, 20, 110, "Press back to exit.");

            // Draw UP label with triangle
            //u8g2_DrawStr(&u8g2, 50, 40, "UP");
            draw_triangle_up(60, 37); // Upward triangle next to UP

            // Draw DOWN label with triangle
            //u8g2_DrawStr(&u8g2, 50, 75, "DOWN");
            draw_triangle_down(60, 77); // Downward triangle next to DOWN


            // Handle text (centered)
            u8g2_DrawStr(&u8g2, 80, 60, "Handle");
            break;
        }
        case FLOAT_MODE: {
            u8g2_DrawStr(&u8g2, 50, 20, "Hold Mode");
            u8g2_DrawStr(&u8g2, 20, 110, "Press back to exit.");

            // Draw R label
            u8g2_DrawStr(&u8g2, 60, 60, "Drag or Rotate");

            // Draw horizontal arrows for R adjustment
            draw_triangle_left(50, 57);  // Left arrow next to R
            draw_triangle_right(90, 57); // Right arrow next to R

            break;
        }
        case CALIBRATION: {
            u8g2_DrawStr(&u8g2, 10, 30, "Initializing");
            u8g2_DrawStr(&u8g2, 10, 60, "Please wait...");
            u8g2_DrawStr(&u8g2, 10, 90, "Do not move device");
            break;
        }
        case PLACE_WEIGHT: {
            //u8g2_DrawStr(&u8g2, 50, 30, "Place Weight");
            u8g2_DrawStr(&u8g2, 20, 60, "Attach the weight");
            break;
        }
        case DONE_CALIBRATION: {
            u8g2_DrawStr(&u8g2, 10, 30, "Weight is Measured");
            u8g2_DrawStr(&u8g2, 10, 60, "Can Lift Now");
            u8g2_DrawStr(&u8g2, 10, 90, "Press UP to safely");
            u8g2_DrawStr(&u8g2, 10, 110, "detach the weight");
            break;
        }
        case PRESS_BUTTON: {
            u8g2_DrawStr(&u8g2, 10, 30, "Press Select Button");
            u8g2_DrawStr(&u8g2, 10, 60, "to Continue");
            break;
        }
        case PLEASE_WAIT: {
            u8g2_DrawStr(&u8g2, 10, 30, "Please Wait");
            u8g2_DrawStr(&u8g2, 10, 60, "Processing...");
            break;
        }
    }
}