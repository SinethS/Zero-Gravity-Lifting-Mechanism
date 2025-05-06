#include "I2C_AVR.h"

void i2c_init(void) {
    TWSR = 0; // Clear status register
    TWBR = ((F_CPU / 100000UL) - 16) / 2; // Set I2C to 100kHz
    TWCR = (1 << TWEN); // Enable TWI
}

void i2c_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}