
#include <avr/io.h>
#include <util/delay.h>
#include "HX711.h"
#include <stdio.h>

void uart_init() {
    UBRR0H = 0;
    UBRR0L = 8; // 115200 baud @ 16MHz
    UCSR0B = (1 << TXEN0);  // Enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void uart_send_char(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_send_string(const char* str) {
    while (*str) {
        uart_send_char(*str++);
    }
}


int main() {
  // Initialize HX711 object
  HX711 hx711(PE4, PE5);

  // Set up serial communication


  uart_init();

  while (true) {
    // Read the raw value from the HX711
    long rawValue = hx711.read();
    _delay_ms(100);  // Add after reading

    // Print the raw value
    char rawValueStr[20];
    sprintf(rawValueStr, "%ld", rawValue);
    uart_send_string(rawValueStr);
    _delay_ms(1000);
  }

  return 0;
}





