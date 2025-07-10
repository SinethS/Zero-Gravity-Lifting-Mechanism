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

class IO;
class ProfileController;
class LinearControl;
class ADS1232;
class TouchController;
class UART;
class Menu;
class motor;

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
    motor *stepper; // Pointer to motor for crane control

    int *button = nullptr; // Pointer to button state

    char buffer[100]; // Buffer for formatted strings

    float linear_value = 0.0f; // Previous linear control value

    int speed = 0;     // Previous speed value
    int prv_speed = 0; // Previous speed for controller mode
    float adc; // Previous ADC value for touch controller
    float weight; // Previous ADC value for touch controller

    const float SPEED_TO_RPM = 10.0f; // Convert TouchController speed to RPM (e.g., 100.0f -> 1000 RPM)

    const float adc_diff = 212989.0f; // ADC difference threshold for stability check

public:
    ControllerUtil(IO *io, ProfileController *profilecontroller, LinearControl *handle_controller, ADS1232 *ads, TouchController *touchController, UART *uart, Menu *menu, motor *stepper, int *button = nullptr );
    void setMenu(Menu* menu_ptr) { this->menu = menu_ptr; }
    void callibrateADS1232_weight(float known_weight = 2500.0f); // Callibrate ADS1232 with a known weight

    void handlLinearControl();
    void handleADS1232Control();
    void handleButtonControl();
    void handleFloatControl(); 
    void initCalibration();
    void zeroGravity();
};

#endif // CONTROLLER_UTILS_H