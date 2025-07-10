#include "touchController.h"

// Constructor implementation
TouchController::TouchController(float marginValue) : initial(0), speed(0.0f), margin(marginValue), error(0) {}

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
float clamp(float value, float min_val, float max_val)
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

    // Calculate error (difference from initial value)
    error = initial - ADC_value;

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