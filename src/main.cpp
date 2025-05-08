#include <avr/io.h>
#include <util/delay.h>
#include "HX711.h"
#include <stdio.h>
#include "UART.h"
#include "I2C.h"

int main() {

  // SET UP 

  // Initialize HX711 object
  HX711 hx711(PE4, PE5);  // PE4 = Data, PE5 = Clock
  
  // Set up serial communication
  UART uart(115200);  // Initialize UART with baud rate 115200

  // Initialize I2C (if needed)
  // TWI_init();

  while (true) {

    // LOOP

    // Read the raw value from the HX711
    long rawValue = hx711.read();

    // Print the raw value
    uart.println(rawValue, 2);  // Print the raw ADC value with 2 decimal places

    // Delay for 10 ms
    _delay_ms(10);
  }

  return 0;
}