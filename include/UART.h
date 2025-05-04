#ifndef _UART_H
#define _UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>



class UART {
public:
    // Constructor with baud rate
    UART();

    // Function to transmit data
    void transmit(unsigned char data);

    // Function to receive data
    unsigned char receive();

    // Function to transmit a string
    void transmitString(const char* str);

    // Function to transmit a number
    void transmitNumber(long number);

    // Function to transmit a float number with specified decimal places
    void transmitFloat(float number, int decimal_places);

    // Function to transmit a string followed by a newline
    void println(const char* str);


};

#endif