#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef MOTOR_H
#define MOTOR_H

#define PRESCALER 8
class motor{

private:
    int current_rpm = 0; // Speed of the motor 
    bool direction = 1; // Direction of the motor (true for forward, false for backward)
    bool running = false; // Motor state (true for running, false for stopped)

    bool int_status = false; // Interrupt flag for Timer5
    bool angle_set = false; // Flag to check if angle is set

    unsigned long microstep = 0; // Microstepping mode 
    int turns = 0; // Number of revolutions 

    void initPWM_TIM1();
    void initCounter_TIM5();

    void stopCounter_TIM5();
    void stopPWM_TIM1();

    void startCounter_TIM5();
    void startPWM_TIM1();

    void resetCounter_TIM5();
    void resetPWM_TIM1();

    void detachINTERUPT_TIM5(); // Detach interrupt for Timer5
    void attachINTERUPT_TIM5(); // Attach interrupt for Timer5

    void setSpeed(unsigned int rpm);
    void setDirection(bool dir);
    void setAngle(unsigned int angle);
    void runMotor();

public:
    motor(unsigned long microstep);
    void initMotor(); // Initialize the motor
    void stopMotor();
    void speedcontrol(int rpm); // Control the motor speed 
    void turnAngle(int angle, unsigned int rpm); // Turn the motor to a specific angle
    void getAngle(); // Get the current angle of the motor
    void resetAngle(); // Reset the angle of the motor

};



#endif