# include "IO.h"

IO::IO() {
    // Constructor
}

void IO::initIO() {
    // Initialize LEDs
    initLEDs();

    // Initialize buttons
    initButtons();
}

void IO::attacthINTERUPT_PCINT1() {
    PCICR |= (1 << PCIE1);         // Enable Pin Change Interrupt for Port J
}

void IO::detachINTERUPT_PCINT1() {
    PCICR &= ~(1 << PCIE1);        // Disable Pin Change Interrupt for Port J
}


void IO::initLEDs() {
    // Set PJ4, PJ5, PJ6, PJ7 as output for LEDs
    DDRJ |= (1 << PJ0) | (1 << PJ5) | (1 << PJ6) | (1 << PJ7); // LED pins
    PORTJ &= ~((1 << PJ0) | (1 << PJ5) | (1 << PJ6) | (1 << PJ7)); // Set all LEDs off
}

void IO::initButtons() {
    // Set PK0, PJ1, PJ2, PJ3 as input for buttons
    DDRJ &= ~((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3)); // Button pins
    PORTJ &= ~((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3)); // Disable pull-ups on buttons

    // Attach Pin Change Interrupt for PK0 to PJ3
    attacthINTERUPT_PCINT1(); // Enable Pin Change Interrupt for Port J
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12);  // Enable specific pins

}


int IO::buttonUpdate() {
    // Read the state of the buttons (PK0 to PJ3)

    interrupt_state = false; // Set the interrupt state to false
    if(millis() - last_press_time < 25) {
        // Debounce time check
        return 0; // No button pressed
    }
    last_press_time = millis(); // Update last press time

    uint8_t current = PINJ & ((1 << PJ0) | (1 << PJ1) | (1 << PJ2) | (1 << PJ3)); // Read PK0 to PJ3
    uint8_t changed = current ^ last_state;
    last_state = current;
    
    int value = 0;

    if (changed & (1 << PJ0)) {
        if (current & (1 << PJ0)) {
            // Button on PK0 pressed (rising edge detected)
            value = 1;
        } else {
            // Button on PK0 released (falling edge detected)
            value = -1;
        }
    } else if (changed & (1 << PJ1)) {
        if (current & (1 << PJ1)) {
            // Button on PJ1 pressed (rising edge detected)
            value = 2;
        } else {
            // Button on PJ1 released (falling edge detected)
            value = -2;
        }
    } else if (changed & (1 << PJ2)) {
        if (current & (1 << PJ2)) {
            // Button on PJ2 pressed (rising edge detected)
            value = 3;
        } else {
            // Button on PJ2 released (falling edge detected)
            value = -3;
        }
    } else if (changed & (1 << PJ3)) {
        if (current & (1 << PJ3)) {
            // Button on PJ3 pressed (rising edge detected)
            value = 4;
        } else {
            // Button on PJ3 released (falling edge detected)
            value = -4;
        }
    }
    return value; // No button pressed
}


void IO::setLED(uint8_t pattern) {
    // Set the LEDs based on the pattern (4 LEDs on PJ4 to PJ7)
    PORTJ = (PORTJ & 0x0F) | ((pattern & 0x0F) << 4);
}

void IO::toggleLED(unsigned int led_number) {
    // Toggle the specific LED
    if (led_number >= 1 && led_number <= 4) {
        PORTJ ^= (1 << (PJ0 + led_number - 1));  // Toggle LED4 to LED7
    }
}

void IO::controlLEDs(uint8_t led, bool pattern) {
    // Control LEDs, either toggle or set to a pattern
    if (pattern) {
        setLED(led);
    } else {
        toggleLED(led);
    }
}
