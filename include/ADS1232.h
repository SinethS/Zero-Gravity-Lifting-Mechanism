#ifndef ADS1232_H
#define ADS1232_H

#include <avr/io.h>
#include <util/delay.h>
#include "UART.h"

void ADS1232_Init();
uint32_t ADS1232_Read();
void ADS1232_StartConversion();
void ADS1232_SetGain(uint8_t gain);
uint32_t ADS1232_GetAverage(uint8_t samples);
void ADS1232_Calibrate(uint32_t *offset, uint32_t *scale);


#endif