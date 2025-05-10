#include "ICM20948.h"

static void ICM_select_bank(uint8_t bank) {
    SPI_write_reg(REG_BANK_SEL, bank << 4);
}

float low_pass_filter(float input, float prev, float alpha) {
    return alpha * input + (1.0f - alpha) * prev;
}

static int16_t combine(uint8_t msb, uint8_t lsb) {
    return (int16_t)((msb << 8) | lsb);
}

void ICM20948_init(void) {
    DDRB |= (1 << PB0);
    CS_HIGH();
    _delay_ms(10);

    ICM_select_bank(0);
    SPI_write_reg(PWR_MGMT_1, 0x01);
    _delay_ms(100);

    ICM_select_bank(3);
    SPI_write_reg(I2C_MST_CTRL, 0x07);
    ICM_select_bank(0);
    SPI_write_reg(USER_CTRL, 0x20);
    _delay_ms(10);

    ICM_select_bank(3);
    SPI_write_reg(I2C_SLV0_ADDR, AK09916_I2C_ADDR);
    SPI_write_reg(I2C_SLV0_REG, AK09916_CNTL2);
    SPI_write_reg(I2C_SLV0_DO, AK09916_MODE_CONTINUOUS_100HZ);
    SPI_write_reg(I2C_SLV0_CTRL, 0x81);
    _delay_ms(10);
}

void ICM20948_read_accel_gyro(float *accel, float *gyro) {
    uint8_t data[12];
    ICM_select_bank(0);
    SPI_read_bytes(ACCEL_XOUT_H, data, 12);

    for (uint8_t i = 0; i < 3; i++) {
        int16_t a = combine(data[i*2], data[i*2+1]);
        accel[i] = low_pass_filter(a * ACCEL_SCALE, i == 0 ? ax_prev : (i == 1 ? ay_prev : az_prev), ALPHA);
        if (i == 0) ax_prev = accel[i];
        else if (i == 1) ay_prev = accel[i];
        else az_prev = accel[i];
    }

    for (uint8_t i = 0; i < 3; i++) {
        int16_t g = combine(data[6 + i*2], data[7 + i*2]);
        gyro[i] = low_pass_filter(g * GYRO_SCALE, i == 0 ? gx_prev : (i == 1 ? gy_prev : gz_prev), ALPHA);
        if (i == 0) gx_prev = gyro[i];
        else if (i == 1) gy_prev = gyro[i];
        else gz_prev = gyro[i];
    }
}

void ICM20948_read_mag(float *mag) {
    uint8_t data[6];

    ICM_select_bank(3);
    SPI_write_reg(I2C_SLV0_ADDR, 0x8C);
    SPI_write_reg(I2C_SLV0_REG, AK09916_DATA);
    SPI_write_reg(I2C_SLV0_CTRL, 0x87);
    _delay_ms(10);

    ICM_select_bank(0);
    SPI_read_bytes(EXT_SLV_SENS_DATA_00, data, 6);

    for (uint8_t i = 0; i < 3; i++) {
        int16_t m = combine(data[i*2+1], data[i*2]);
        mag[i] = low_pass_filter(m * MAG_SCALE, i == 0 ? mx_prev : (i == 1 ? my_prev : mz_prev), ALPHA);
        if (i == 0) mx_prev = mag[i];
        else if (i == 1) my_prev = mag[i];
        else mz_prev = mag[i];
    }
}
