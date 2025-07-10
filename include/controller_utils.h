#include "motor.h"
#include "linearControl.h"
#include "ADS1232.h"
#include "IO.h"
#include "UART.h"
#include "timemillis.h"
#include "touchController.h"
#include "profilecontroller.h"
#include "menu.h"

#ifndef CONTROLLER_UTILS_H
#define CONTROLLER_UTILS_H

class Menu; // Forward declaration of Menu class

class ControllerUtil
{

    private:
        IO *io;
        ProfileController *profilecontroller; // Profile controller for crane control
        LinearControl *handle_controller;
        ADS1232 *ads;
        TouchController *touchController; // Touch controller for crane control
        UART *uart;
        Menu *menu; // Pointer to menu for displaying information

        int *button = nullptr; // Pointer to button state

        char buffer[100]; // Buffer for formatted strings

        float linear_value = 0.0f; // Previous linear control value

        int speed = 0;     // Previous speed value
        int prv_speed = 0; // Previous speed for controller mode

        int prev_adc; // Previous ADC value for touch controller
        int current_adc; // Current ADC value for touch controller

        const float SPEED_TO_RPM = 10.0f; // Convert TouchController speed to RPM (e.g., 100.0f -> 1000 RPM)

    public:
        ControllerUtil(IO *io, ProfileController *profilecontroller, LinearControl *handle_controller, ADS1232 *ads, TouchController *touchController, UART *uart, int *button = nullptr );
        void callibrateADS1232_weight(float known_weight = 2500.0f); // Callibrate ADS1232 with a known weight
        void addMenu(Menu *menu);
        void handlLinearControl();
        void handleADS1232Control();
        void handleButtonControl();
        void handleFloatControl(); 
};

#endif // CONTROLLER_UTILS_H