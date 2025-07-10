#ifndef TOUCHCONTROLLER_H
#define TOUCHCONTROLLER_H

#include <stdint.h>

class TouchController
{
private:
    float initial;      // Initial touch position/value
    float speed;        // Touch speed
    const float margin; // Constant margin value
    //uint32_t error;     // Error value for PID control
    float error;     // Error value for PID control

        // Static variables to store PID state
    float prev_error = 0.0f;
    float integral = 0.0f;
    float prev_derivative = 0.0f;

        // PID constants (tune these based on system response)
    const float Kp = 0.05f;   // Proportional gain
    const float Ki = 0.00f;   // Integral gain
    const float Kd = 0.00f;   // Derivative gain
    const float ALPHA = 0.1f; // Low-pass filter coefficient for derivative (0 < ALPHA < 1)

    // System constraints
    const float SPEED_SCALE = 0.07f;   // Speed scaling factor
    const float MAX_SPEED = 300.0f;    // Maximum upward speed
    const float MIN_SPEED = -300.0f;   // Maximum downward speed
    const float INTEGRAL_MAX = 100.0f; // Anti-windup limit for integral term



public:
    // Constructor
    TouchController(float marginValue = 10000.0f);

    // Get initial value
    float getInitial() const;

    // Get speed value
    float getSpeed() const;

    float getMargin() const;

    float getError() const { return error; }

    // Update both initial and speed values
    void updateInitial(float newInitial);
    void updateSpeed(float ADC_value);
};

#endif // TOUCHCONTROLLER_H