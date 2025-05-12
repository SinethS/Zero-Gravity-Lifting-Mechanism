#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor.h"
#include "ICM20948.h"

volatile bool loop_flag = false; // Flag to indicate when to loop

void setupTimer2();
void setupTimer3();
void setupTimer5();

#endif // TIMER_H