#include "motor.h"
#include "linearControl.h"
#include "ADS1232.h"
#include "IO.h"
#include "UART.h"
#include "timemillis.h"

#ifndef CONTROLLER_UTILS_H
#define CONTROLLER_UTILS_H

class ControllerUtil{

    private:
        IO *io;
        motor *stepper;
        LinearControl *handle_controller;
        ADS1232 *ads;
        UART *uart;

        int *button = nullptr; // Pointer to button state

        char buffer[100]; // Buffer for formatted strings


        float linear_value = 0.0f; // Previous linear control value

        int speed = 0; // Previous speed value
        int prv_speed = 0; // Previous speed for controller mode

    public:

        ControllerUtil(IO *io, motor *stepper, LinearControl *handle_controller, ADS1232 *ads, UART *uart, int *button = nullptr);
    
        void callibrateADS1232_weight(float known_weight = 2500.0f); // Callibrate ADS1232 with a known weight

        void handlLinearControl();
        void handleADS1232Control();
        void handleButtonControl();


};


#endif // CONTROLLER_UTILS_H