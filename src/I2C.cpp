// src/I2C.cpp

// CORRECTED: Include the header file we just created.
#include "I2C.h"

// ADDED: Include the delay header for _delay_ms() and _delay_us().
#include <util/delay.h>

/**
 * @brief Initializes the TWI (I2C) peripheral.
 * The formula is: SCL freq = F_CPU / (16 + 2 * TWBR * Prescaler)
 * With F_CPU=16MHz, Prescaler=1, and TWBR=32:
 * 16,000,000 / (16 + 2 * 32 * 1) = 16,000,000 / 80 = 200,000 Hz (200 kHz)
 * Your original comment said 100kHz, but the value was for 200kHz.
 * For 100kHz, use TWBR = 72. For 400kHz, use TWBR = 12.
 */
void TWI_init(void) {
    // Set bit rate register
    TWSR = 0x00; // Prescaler = 1
    TWBR = 32;   // SCL freq ~ 200kHz
}

/**
 * @brief This is the initialization function specifically for the U8g2 library setup.
 * It calls the generic TWI_init.
 */
void i2c_avr_init(void) {
    TWI_init();
}

void TWI_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Send START
    while (!(TWCR & (1 << TWINT)));                   // Wait for complete
}

void TWI_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Send STOP
    // CORRECTED: Wait for the STOP bit to be cleared by hardware
    // while (TWCR & (1 << TWSTO)); // This can hang, the bit is cleared automatically
}

void TWI_write(uint8_t data) {
    TWDR = data;                                      // Load data
    TWCR = (1 << TWINT) | (1 << TWEN);                // Start transmission
    while (!(TWCR & (1 << TWINT)));                   // Wait for complete
}

uint8_t TWI_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);  // Read with ACK
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t TWI_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);                // Read with NACK
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t TWI_get_status(void) {
    return TWSR & 0xF8; // Mask prescaler bits
}

// U8g2 I2C byte communication callback
uint8_t u8x8_byte_avr_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    uint8_t *data;
    switch (msg) {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;
            while (arg_int > 0) {
                TWI_write(*data);
                data++;
                arg_int--;
            }
            break;
        case U8X8_MSG_BYTE_INIT:
            // TWI_init() is called separately by i2c_avr_init() during setup
            break;
        case U8X8_MSG_BYTE_SET_DC:
            // Not used in I2C
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            TWI_start();
            TWI_write(u8x8_GetI2CAddress(u8x8));
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            TWI_stop();
            break;
        default:
            return 0;
    }
    return 1;
}

// U8g2 GPIO and delay callback
uint8_t u8x8_gpio_and_delay_avr(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            // Not used for HW I2C
            break;
        case U8X8_MSG_DELAY_MILLI:
            while(arg_int--) {
              _delay_ms(1);
            }
            break;
        // CORRECTED: The original code used a 1us delay instead of 10us
        case U8X8_MSG_DELAY_10MICRO:
            while(arg_int--) {
              _delay_us(10);
            }
            break;
        case U8X8_MSG_DELAY_I2C: // Used for software I2C bit-banging
            // For hardware I2C, this can be short. The TWI hardware handles timing.
             _delay_us(1);
            break;
        default:
            // u8x8_SetGPIOResult(u8x8, 1); // For GPIO operations, not needed here
            break;
    }
    return 1;
}