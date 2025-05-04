#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef MOTOR_H
#define MOTOR_H

#define PRESCALER 8
class motor{

private:
    int current_rpm = 0; // Speed of the motor (0-255)
    bool direction = 1; // Direction of the motor (true for forward, false for backward)
    bool running = false; // Motor state (true for running, false for stopped)

    unsigned long microstep = 0; // Microstepping mode (0-255)

    void initPWM_TIM1();
    void initCounter_TIM5();

    void stopCounter_TIM5();
    void stopPWM_TIM1();

    void startCounter_TIM5();
    void startPWM_TIM1();

    void resetCounter_TIM5();
    void resetPWM_TIM1();


public:
    motor(unsigned long microstep);
    void initMotor(); // Initialize the motor
    void setSpeed(unsigned int rpm);
    void setDirection(bool dir);
    void setAngle(unsigned int angle);
    void stopMotor();
    void runMotor();
    void control(int rpm, unsigned int angle = 0); // Control the motor speed and angle
};



#endif