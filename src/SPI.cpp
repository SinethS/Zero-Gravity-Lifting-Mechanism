#include "SPI.h"

void SPI_init(void) {
    DDRB |= (1<<PB2)|(1<<PB1)|(1<<PB0); // MOSI, SCK, CS
    DDRB &= ~(1<<PB3); // MISO
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    CS_HIGH();
}

uint8_t SPI_transfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1<<SPIF)));
    return SPDR;
}

void SPI_write_reg(uint8_t reg, uint8_t data) {
    CS_LOW();
    SPI_transfer(reg & 0x7F);
    SPI_transfer(data);
    CS_HIGH();
}

uint8_t SPI_read_reg(uint8_t reg) {
    CS_LOW();
    SPI_transfer(reg | 0x80);
    uint8_t value = SPI_transfer(0xFF);
    CS_HIGH();
    return value;
}

void SPI_read_bytes(uint8_t reg, uint8_t *buffer, uint8_t len) {
    CS_LOW();
    SPI_transfer(reg | 0x80);
    for (uint8_t i = 0; i < len; i++)
        buffer[i] = SPI_transfer(0xFF);
    CS_HIGH();
}
