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

// defines end

// variable declarations


motor stepper(1600);  // Initialize motor with 1600 microsteps
UART uart(115200);


// ISR(TIMER2_COMPA_vect) {
//   loop_flag = true;  // Set flag every 8 ms
// }

ISR(TIMER5_COMPA_vect) {
  stepper.stopMotor();  // Stop motor on compare match
  uart.println("Motor stopped"); 
}


int main(void) {

  TWI_init();
  SPI_init();
  ICM20948_init();


  uart.transmitString("hello world!");  // Send message over UART

  setupTimer2();  // Initialize Timer2
  stepper.initMotor();  // Initialize motor
  uart.println("Motor initialized");  // Send message over UART
  // stepper.speedcontrol(-100);  // Set speed to 100 RPM
  stepper.turnAngle(-360, 60);    
  stepper.ENmotor();  // Enable motor



  while (1) {
      // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
      if(loop_flag) {
          loop_flag = false;  // Reset flag
          char buffer[50];
          snprintf(buffer, sizeof(buffer), "Angle: %.2f \n", stepper.getAngle());  // Get angle from motor
          uart.transmitString(buffer);  // Send angle over UART
          // loop code begin

          // loop code end
      }
  }
}






