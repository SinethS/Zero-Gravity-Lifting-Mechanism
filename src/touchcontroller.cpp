#include "touchController.h"
#include "stdint.h"

// Constructor implementation
TouchController::TouchController(float marginValue) : initial(0.0f), speed(0.0f), margin(marginValue) {}

// Get initial value implementation
float TouchController::getInitial() const
{
    return initial;
}

// Get speed value implementation
float TouchController::getSpeed() const
{
    return int(speed);
}

// Get margin value implementation
float TouchController::getMargin() const
{
    return margin;
}

// Update initial value implementation
void TouchController::updateInitial(float newInitial)
{
    initial = newInitial;
}

// Custom clamp function to replace std::clamp
float clamp(uint32_t value, float min_val, float max_val)
{
    if (value < min_val)
        return min_val;
    if (value > max_val)
        return max_val;
    return value;
}

// Update speed based on ADC value for crane control using PID
void TouchController::updateSpeed(float ADC_value)
{
    // PID constants (tune these based on system response)
    const float Kp = 0.05f;   // Proportional gain
    const float Ki = 0.01f;   // Integral gain
    const float Kd = 0.02f;   // Derivative gain
    const float ALPHA = 0.1f; // Low-pass filter coefficient for derivative (0 < ALPHA < 1)

    // System constraints
    const float SPEED_SCALE = 0.05f;   // Speed scaling factor
    const float MAX_SPEED = 400.0f;    // Maximum upward speed
    const float MIN_SPEED = -400.0f;   // Maximum downward speed
    const float INTEGRAL_MAX = 100.0f; // Anti-windup limit for integral term

    // Static variables to store PID state
    static float prev_error = 0.0f;
    static float integral = 0.0f;
    static float prev_derivative = 0.0f;

    // Calculate error (difference from initial value)
    uint32_t error = initial - ADC_value;

    if (error > margin || error < -margin)
    {
        // Proportional term
        float P = Kp * error;

        // Integral term with anti-windup
        integral += error;
        integral = clamp(integral, -INTEGRAL_MAX, INTEGRAL_MAX); // Limit integral to prevent windup
        float I = Ki * integral;

        // Derivative term with low-pass filter
        float derivative = (error - prev_error);
        derivative = ALPHA * derivative + (1.0f - ALPHA) * prev_derivative; // Low-pass filter
        float D = Kd * derivative;

        // Update previous values for next iteration
        prev_error = error;
        prev_derivative = derivative;

        // Calculate speed using PID output
        speed = (P + I + D) * SPEED_SCALE;

        // Clamp speed to system constraints
        speed = clamp(speed, MIN_SPEED, MAX_SPEED);
    }
    else
    {
        // Within margin: Stop crane and reset PID terms
        speed = 0.0f;
        integral = 0.0f; // Reset integral to prevent windup
        prev_error = 0.0f;
        prev_derivative = 0.0f;
    }
}