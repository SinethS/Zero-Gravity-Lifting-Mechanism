#ifndef _UART_H
#define _UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>



class UART {
public:

    UART(uint64_t baudRate);
    void transmit(unsigned char data);
    unsigned char receive();
    void transmitString(const char* str);
    void transmitNumber(long number);
    void transmitFloat(float number, int decimal_places);
    void print(const char* str);
    void print(long number);
    void print(float number, int decimal_places);
    void println(const char* str);
    void println(long number);
    void println(float number, int decimal_places);
    void print_hex(uint8_t number);
    void println_hex(uint8_t number);

};

#endif