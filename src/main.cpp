#include <avr/io.h>
#include <util/delay.h>
#include "HX711.h"
#include <stdio.h>
#include "UART.h"
#include "I2C.h"


int main() {

  // SET UP 

  // Initialize HX711 object
  // HX711 hx711(PE4, PE5);
  
  // Set up serial communication
  UART uart(115200);  // Initialize UART with baud rate 115200

  // Initialize I2C
  // TWI_init();

  while (true) {

    // LOOP

    // Read the raw value from the HX711
    // long rawValue = hx711.read();
    // _delay_ms(100);  // Add after reading

    // Print the raw value
    
    uart.println(12.5, 2);
    
    _delay_ms(1000);
  }

  return 0;
}





