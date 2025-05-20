#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "UART.h"
#include "IO.h"
#include "io_portk.h"
#include "timemillis.h"
#include "I2C.h"
#include "HX711.h"


// defines

// defines end

// variable declarations

motor stepper(1600);  // Initialize motor with 1600 microsteps
UART uart;  // Initialize UART
// IO io;  // Initialize IO
IO_PortK io_k;
HX711 hx711;  // Initialize HX711

// varible declarations end


// function declarations

ISR(INT4_vect) {
    hx711.detach_interrupt();  // Detach interrupt to prevent re-entrance
    bool hx711_data_ready = hx711.is_ready();  // Check if HX711 data is ready
    if(hx711_data_ready) {
        hx711.read();  // Read raw value from HX711
    }
    hx711.attach_interrupt();  // Detach interrupt to prevent re-entrance
}

ISR(TIMER5_COMPA_vect) {
    stepper.stopMotor();  // Stop motor on compare match
    uart.println("Motor stopped"); 
}

ISR(PCINT2_vect){
    io_k.detachInterrupt_PCINTK();  // Detach interrupt for Port K
    int x = io_k.buttonUpdate();  // Update button state
    // create and put update display or get button input display
    uart.transmitNumber(x);  // Send button state over UART
    io_k.attachInterrupt_PCINTK();  // Reattach interrupt for Port K
}

// function declarations end


int main(void) {

    uart.transmitString("hello world!");  // Send message over UART

    timer2_ctc_100hz_init();  // Initialize Timer2 for 100 Hz
    stepper.initMotor();  // Initialize motor
    uart.println("Motor initialized");  // Send message over UART
    millis_init();  // Initialize millis
    uart.println("Millis initialized");  // Send message over UART
    io_k.initIO();  // Initialize IO
    uart.println("IO initialized");  // Send message over UART
    // Initialize HX711 object
    hx711.init_HX711(PE4, PE5);  // PE4 = Data, PE5 = Clock

    stepper.speedcontrol(60);
    stepper.ENmotor();


    while (1) {
        // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
        if(get_flag()) {  // Check if loop flag is set
            clear_flag();  // Clear loop flag
            // uart.println("Looping...");  // Send message over UART
            char buffer[50];
            // sprintf(buffer, "Time: %lu ms\n", millis());  // Get current time in milliseconds
            // uart.transmitString(buffer);  // Send time over UART
            // loop code begin

            // sprintf(buffer, "Raw Value: %ld\n", hx711.get_raw_value());  // Get raw value from HX711
            // uart.transmitString(buffer);  // Send raw value over UART
            // loop code end
        }
    }
}
