#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>

#define CS_LOW()   (PORTB &= ~(1 << PB0))
#define CS_HIGH()  (PORTB |= (1 << PB0))

void SPI_init(void);
uint8_t SPI_transfer(uint8_t data);
void SPI_write_reg(uint8_t reg, uint8_t data);
uint8_t SPI_read_reg(uint8_t reg);
void SPI_read_bytes(uint8_t reg, uint8_t *buffer, uint8_t len);

#endif