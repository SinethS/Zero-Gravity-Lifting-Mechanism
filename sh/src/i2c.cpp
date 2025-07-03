#include "i2c.h" // Updated include
#include <avr/io.h>
#include <util/twi.h>

// Constructor: Initializes I2C communication
I2C::I2C() {
    // Set TWI clock to 100 kHz for a 16MHz F_CPU
    TWSR = 0; // Prescaler = 1
    TWBR = 72;
    // Enable TWI
    TWCR = (1 << TWEN);
}

void I2C::start() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void I2C::stop() {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void I2C::write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

uint8_t I2C::readAck() {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t I2C::readNack() {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}