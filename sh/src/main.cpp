#define F_CPU 16000000UL

#include "i2c.h"      // Updated include
#include "sh1106.h"   // Updated include
#include <util/delay.h>

int main(void) {
    // Create an I2C bus object. Its constructor initializes the TWI hardware.
    I2C i2c_bus;
    
    // Create a display object, passing it a reference to the I2C bus.
    SH1106 display(i2c_bus);
    
    // Clear the display
    display.clear();
    
    // Draw text using the display object's methods
    display.drawString("Hello from C++!", 0, 0);
    
    while (1) {
        // Main loop
    }

    return 0;
}