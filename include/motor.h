#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef MOTOR_H
#define MOTOR_H

#define F_CPU 16000000UL  // 16 MHz system clock
#define PRESCALER 8
class motor{

private:
    int current_rpm = 0; // Speed of the motor (0-255)
    bool direction = 1; // Direction of the motor (true for forward, false for backward)

    unsigned long microstep = 0; // Microstepping mode (0-255)

    void initPWM_TIM1();
    void initCounter_TIM5();


public:
    motor(unsigned long microstep);
    void initMotor(); // Initialize the motor
    void setSpeed(int rpm);
    void setDirection(bool dir);
    void setAngle(int angle);
    void stopMotor();
    void runMotor();

};


#endif