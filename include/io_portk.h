#ifndef IO_PORTK_H
#define IO_PORTK_H

#include <avr/io.h>
#include <avr/interrupt.h>

class IO_PortK {
private:
    volatile uint8_t last_state;

    void initLEDs();                // Initialize LEDs on Port K
    void initButtons();             // Initialize buttons on Port K
    void toggleLED(unsigned int led); // Toggle LED
    void setLED(uint8_t led);       // Set LED pattern

    void attachInterrupt_PCINTK();  // Attach interrupt for Pin Change Interrupt on Port K
    void detachInterrupt_PCINTK(); // Detach interrupt for Pin Change Interrupt on Port K

public:
    IO_PortK();
    void initIO();                  // Initialize IO ports
    int buttonUpdate();             // Update button states and return actions
    void controlLEDs(uint8_t led, bool pattern = false);  // Control LED pattern (toggle/set)
};

#endif // IO_PORTK_H
