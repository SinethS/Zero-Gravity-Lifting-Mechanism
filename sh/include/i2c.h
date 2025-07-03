#ifndef I2C_H
#define I2C_H

#include <stdint.h>

class I2C {
public:
    // Constructor handles initialization
    I2C();

    void start();
    void stop();
    void write(uint8_t data);
    uint8_t readAck();
    uint8_t readNack();
};

#endif // I2C_H