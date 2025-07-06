#include <avr/io.h>
#include <avr/interrupt.h>
#include "timemillis.h"

#ifndef IO_H
#define IO_H

class IO{

    private:

    
    volatile uint8_t last_state;
    bool interrupt_state; // State of the interrupt

    unsigned long pressed_times[4]; // Array to store pressed times for each button

    unsigned long last_press_time; // Last time a button state was updated

    void initLEDs();                // Initialize LEDs on Port K
    void initButtons();             // Initialize buttons on Port K
    void toggleLED(unsigned int led); // Toggle LED
    void setLED(uint8_t led);       // Set LED pattern

    public:

        IO();
        void initIO();
        int buttonUpdate();
        void controlLEDs(uint8_t led, bool pattern = false); // toggle a specific LED or set a specific LED pattern(set pattern to true)

        void attacthINTERUPT_PCINT1(); // Attach interrupt for Pin Change Interrupt 1
        void detachINTERUPT_PCINT1(); // Detach interrupt for Pin Change Interrupt 1

        

};

#endif // IO_H