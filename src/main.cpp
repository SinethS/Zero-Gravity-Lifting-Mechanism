#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#define F_CPU 16000000UL  // 16 MHz system clock
#define PRESCALER 8

unsigned long baudRate = 115200;  // Baud rate for UART  



volatile bool loop_flag = false;

void timer2_ctc_100hz_init(void) {
    TCCR2A = (1 << WGM21);  // CTC mode
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS22);  // 1024
    

    // (F_CPU / (Prescaler * Frequency)) - 1 = (16e6 / (64 * 100)) - 1 = 249
    OCR2A = 249;

    TIMSK2 = (1 << OCIE2A);  // Enable compare match interrupt
    sei();  // Enable global interrupts
}



// Function to initialize Timer1 in CTC mode with given frequency
void timer1_ctc_init() {
    // Set OC1A (PB5) as output
    DDRB |= (1 << PB5);

    // Set CTC mode: WGM12 = 1 (Mode 4), toggle OC1A on match
    TCCR1A = (1 << COM1A0);                     // Toggle OC1A on compare match
    TCCR1B = (1 << WGM12) | (1 << CS11);        // CTC mode, Prescaler = 8

    // Calculate and set OCR1A for desired frequency
    // OCR1A = (F_CPU / (2 * PRESCALER * frequency)) - 1;
}


volatile uint32_t match_event_count = 0;




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

ISR(TIMER2_COMPA_vect) {
    loop_flag = true;  // Set flag every 10 ms
    uart_send_string("10 ms tick\r\n");  // Send message over UART
}


ISR(TIMER5_COMPA_vect) {
    // Triggered every 100 rising edges on Pin 47
    match_event_count++;  // Increment match event count
}

int main(void) {

    char buffer[50];  // Buffer for UART string
    snprintf(buffer, sizeof(buffer), "Hello, World!\r\n");

    timer1_ctc_init();  // 1 kHz
    timer2_ctc_100hz_init();  // Initialize Timer2 for 100 Hz

    uart_init();  // Initialize UART

    uart_send_string(buffer);  // Send initial message



    while (1) {
        // Loop forever — frequency generation is hardware-driven
        if(loop_flag) {
            loop_flag = false;  // Reset flag
            // sprintf(buffer, "Match Count: %lu\r\n", match_event_count);
            // uart_send_string(buffer);  // Send match count over UART
            // Do something every 10 ms
            // e.g., toggle an LED, send data, etc.
        }
    }
}
