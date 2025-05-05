#ifndef MPU9250_H
#define MPU9250_H

#include <stdint.h>

#define MPU9250_ADDR  0x68
#define AK8963_ADDR   0x0C

void mpu9250_init(void);
void mpu9250_read_accel(int16_t *ax, int16_t *ay, int16_t *az);
void mpu9250_read_gyro(int16_t *gx, int16_t *gy, int16_t *gz);
void mpu9250_read_mag(int16_t *mx, int16_t *my, int16_t *mz);

#endif
