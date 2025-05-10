#ifndef ICM20948_H
#define ICM20948_H

#include <avr/io.h>
#include "SPI.h"
#include <util/delay.h>

#define CS_LOW()   (PORTB &= ~(1 << PB0))
#define CS_HIGH()  (PORTB |= (1 << PB0))

#define WHO_AM_I             0x00
#define REG_BANK_SEL         0x7F
#define PWR_MGMT_1           0x06
#define ACCEL_XOUT_H         0x2D
#define GYRO_XOUT_H          0x33
#define EXT_SLV_SENS_DATA_00 0x3B
#define USER_CTRL            0x03
#define I2C_MST_CTRL         0x01
#define I2C_SLV0_ADDR        0x03
#define I2C_SLV0_REG         0x04
#define I2C_SLV0_DO          0x06
#define I2C_SLV0_CTRL        0x05

#define AK09916_I2C_ADDR     0x0C
#define AK09916_DATA         0x11
#define AK09916_CNTL2        0x31
#define AK09916_MODE_CONTINUOUS_100HZ 0x08

#define ACCEL_SCALE (9.81f / 16384.0f)
#define GYRO_SCALE  (250.0f / 32768.0f)
#define MAG_SCALE   (0.15f)
#define ALPHA       0.1f

static float ax_prev = 0, ay_prev = 0, az_prev = 0;
static float gx_prev = 0, gy_prev = 0, gz_prev = 0;
static float mx_prev = 0, my_prev = 0, mz_prev = 0;





#endif