#ifndef _I2C_AVR_H_
#define _I2C_AVR_H_

#include <avr/io.h>
#include <stdint.h>

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);

#endif // _I2C_AVR_H_