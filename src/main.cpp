#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "UART.h"
#include "I2C.h"
#include "SPI.h"
#include "ICM20948.h"


int main() {
  UART uart(115200);
  TWI_init();
  SPI_init();
  ICM20948_init();


  
  while (true) {


  }
  return 0;
}





