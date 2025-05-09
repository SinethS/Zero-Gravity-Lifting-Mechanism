#include <avr/io.h>
#include <util/delay.h>
#include "HX711.h"
#include <stdio.h>
#include "UART.h"
#include "I2C.h"
#include "MPU9250.h"


int main() {

  // SET UP 

  // Initialize HX711 object
  // HX711 hx711(PE4, PE5);
  
  // Set up serial communication
  UART uart(115200);  // Initialize UART with baud rate 115200

  //Initialize I2C
  TWI_init();


  // uart.println("Scanning I2C bus...");  only for testing I2C

  // for (uint8_t addr = 1; addr < 127; addr++) {
  //   TWI_start();
  //   int writeResult = TWI_write((addr << 1));
  //   if (writeResult == 0) {  // ACK received
  //     uart.print("Found device at 0x");
  //     uart.print_hex(addr);
  //     uart.println("");
  //   }
  //   TWI_stop();
  // }

  // uart.println("Scan complete.");

  // Initialize MPU9250
  mpu9250_init();

  
  while (true) {

    // LOOP

    // Read the raw value from the HX711
    // long rawValue = hx711.read();
    // _delay_ms(100);  // Add after reading

    // Print the raw value
    
    // uart.println(12.5, 2);
    
    // _delay_ms(1000);

    // Read accelerometer data
    float ax, ay, az;
    mpu9250_read_accel(&ax, &ay, &az);
    uart.print("Accel: ");
    uart.print("X: ");
    uart.print(ax,2);
    uart.print(" Y: ");
    uart.print(ay,2);
    uart.print(" Z: ");
    uart.println(az,2);
    _delay_ms(100);
    // Read gyroscope data
    float gx, gy, gz;
    mpu9250_read_gyro(&gx, &gy, &gz);
    uart.print("Gyro: ");
    uart.print("X: ");
    uart.print(gx,2);
    uart.print(" Y: ");
    uart.print(gy,2);
    uart.print(" Z: ");
    uart.println(gz,2);
    _delay_ms(100);
  }

  return 0;
}





