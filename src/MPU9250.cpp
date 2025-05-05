#include "MPU9250.h"
#include "I2C.h"
#include <util/delay.h>

static uint8_t read_register(uint8_t addr, uint8_t reg) {
    TWI_start();
    TWI_write((addr << 1) | 0);
    TWI_write(reg);
    TWI_start();
    TWI_write((addr << 1) | 1);
    uint8_t data = TWI_read_nack();
    TWI_stop();
    return data;
}

static void write_register(uint8_t addr, uint8_t reg, uint8_t data) {
    TWI_start();
    TWI_write((addr << 1) | 0);
    TWI_write(reg);
    TWI_write(data);
    TWI_stop();
}

static void read_bytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t len) {
    TWI_start();
    TWI_write((addr << 1) | 0);
    TWI_write(reg);
    TWI_start();
    TWI_write((addr << 1) | 1);
    for (uint8_t i = 0; i < len - 1; i++)
        buffer[i] = TWI_read_ack();
    buffer[len-1] = TWI_read_nack();
    TWI_stop();
}

void mpu9250_init(void) {
    TWI_init();
    _delay_ms(100);
    write_register(MPU9250_ADDR, 0x6B, 0x00); // Wake up
    _delay_ms(100);
    write_register(MPU9250_ADDR, 0x1B, 0x00); // Gyro full scale ±250dps
    write_register(MPU9250_ADDR, 0x1C, 0x00); // Accel full scale ±2g

    // Enable bypass to access magnetometer
    write_register(MPU9250_ADDR, 0x37, 0x02);
    _delay_ms(10);
    
    write_register(AK8963_ADDR, 0x0A, 0x16); // 16-bit, continuous mode 2
    _delay_ms(10);
}

void mpu9250_read_accel(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buf[6];
    read_bytes(MPU9250_ADDR, 0x3B, buf, 6);
    *ax = ((int16_t)buf[0] << 8) | buf[1];
    *ay = ((int16_t)buf[2] << 8) | buf[3];
    *az = ((int16_t)buf[4] << 8) | buf[5];
}

void mpu9250_read_gyro(int16_t *gx, int16_t *gy, int16_t *gz) {
    uint8_t buf[6];
    read_bytes(MPU9250_ADDR, 0x43, buf, 6);
    *gx = ((int16_t)buf[0] << 8) | buf[1];
    *gy = ((int16_t)buf[2] << 8) | buf[3];
    *gz = ((int16_t)buf[4] << 8) | buf[5];
}

void mpu9250_read_mag(int16_t *mx, int16_t *my, int16_t *mz) {
    uint8_t buf[7];
    read_bytes(AK8963_ADDR, 0x03, buf, 7);
    if (!(buf[6] & 0x08)) {
        *mx = ((int16_t)buf[1] << 8) | buf[0];
        *my = ((int16_t)buf[3] << 8) | buf[2];
        *mz = ((int16_t)buf[5] << 8) | buf[4];
    }
}
