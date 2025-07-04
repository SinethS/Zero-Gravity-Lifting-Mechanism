// include/I2C.h

#ifndef I2C_H
#define I2C_H

// ADDED: Include U8g2lib header to define u8x8_t and related types/macros.
// This fixes the "'u8x8_t' was not declared" error.
#include <U8g2lib.h> 

// ADDED: Include standard AVR I/O header.
#include <avr/io.h>

// I2C/TWI Configuration
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define SCL_CLOCK 400000L // Note: Your TWBR setting results in 200kHz, not 400kHz.

// --- Function Declarations ---

// General Purpose TWI/I2C Functions
void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t data);
uint8_t TWI_read_ack(void);
uint8_t TWI_read_nack(void);
uint8_t TWI_get_status(void);

// U8g2 Callback Functions
// This is the I2C initialization function your other code is likely calling.
void i2c_avr_init(void);

// U8g2 callback to handle byte-level I2C communication on AVR.
uint8_t u8x8_byte_avr_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

// U8g2 callback to handle all delays.
uint8_t u8x8_gpio_and_delay_avr(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif // I2C_H