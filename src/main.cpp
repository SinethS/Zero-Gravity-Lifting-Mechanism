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

   uint8_t counter = 0;

    while (1) {
        // Continuously send data.
        float accel[3], gyro[3], mag[3];
        ICM20948_read_accel_gyro(accel, gyro);
        char buffer[100];
        sprintf(buffer, "Counter: %d, Accel: [%.2f, %.2f, %.2f], Gyro: [%.2f, %.2f, %.2f], Mag: [%.2f, %.2f, %.2f]\n",
                counter++, accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2]);
        uart.transmitString(buffer);

    }
}






