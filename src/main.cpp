#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "I2C.h"
#include "SPI.h"
#include "ICM20948.h"
#include "motor.h"
#include "timer.h"

// defines
#define WHO_AM_I_REG 0x00  // WHO_AM_I register address for ICM20948
// defines end

// variable declarations


// motor stepper(1600);  // Initialize motor with 1600 microsteps
UART uart(115200);


// ISR(TIMER2_COMPA_vect) {
//   loop_flag = true;  // Set flag every 8 ms
// }

// ISR(TIMER5_COMPA_vect) {
//   stepper.stopMotor();  // Stop motor on compare match
//   uart.println("Motor stopped"); 
// }


int main(void) {
    // Keep your initializations
    SPI_init();
    ICM20948_init();
    uart.transmitString("hello world!\n");

    uint8_t who_am_i_val = 0;

    while (1) {
        // Switch to User Bank 0 (where WHO_AM_I is)
        ICM_select_bank(0);
        _delay_ms(1); // Small delay

        // Read the WHO_AM_I register
        who_am_i_val = SPI_read_reg(WHO_AM_I_REG);

        // Print the result to UART to confirm
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "WHO_AM_I: 0x%02X\n", who_am_i_val);
        uart.transmitString(buffer);

        _delay_ms(500); // Repeat every half second
    }
}






