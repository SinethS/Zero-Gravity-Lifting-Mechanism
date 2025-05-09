#ifndef MPU9250_H
#define MPU9250_H

#include <stdint.h>
#include "I2C.h"

#define MPU9250_ADDR  0x68
#define AK8963_ADDR   0x0C

void mpu9250_init(void);
void mpu9250_read_accel(float *ax, float *ay, float *az);
void mpu9250_read_gyro(float *gx, float *gy, float *gz);
void mpu9250_read_mag(float *mx, float *my, float *mz);
float scale_accel(float raw_value);
float scale_gyro(float raw_value);
float scale_mag(float raw_value);

#endif
