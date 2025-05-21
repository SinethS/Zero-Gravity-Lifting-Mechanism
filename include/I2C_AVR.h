#ifndef _I2C_AVR_H_
#define _I2C_AVR_H_

#include <avr/io.h>
#include <util/delay.h>

void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
int TWI_write(uint8_t data);
uint8_t TWI_read_ack(void);
uint8_t TWI_read_nack(void);
uint8_t TWI_get_status(void);

#endif // _I2C_AVR_H_