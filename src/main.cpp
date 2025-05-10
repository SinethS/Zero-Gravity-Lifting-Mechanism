#include <avr/io.h>
#include <util/delay.h>
#include "HX711.h"
#include <stdio.h>
#include "UART.h"
#include "I2C.h"

int main()
{
  // SET UP

  // Initialize HX711 object
  HX711 hx711(2, 3); // D2 = Data, D3 = Clock

  // Set up serial communication
  UART uart(115200); // Initialize UART with baud rate 115200

  // Main loop
  while (true)
  {
    // Read the raw value from the HX711
    long rawValue = hx711.read(); // Read average of 10 samples
    uart.println(rawValue); // Print the raw value

    _delay_ms(10);
  }

  return 0;
}