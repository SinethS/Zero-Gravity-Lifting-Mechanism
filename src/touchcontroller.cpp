#include "touchController.h"

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

// Update speed based on ADC value for crane control
void TouchController::updateSpeed(float ADC_value)
{
    // Calculate difference from initial value
    float diff = initial - ADC_value;

    // Define constants for crane control (adjust based on hardware)
    const float SPEED_SCALE = 0.01f; // Speed per ADC unit (e.g., 0.1 units/ADC)
    const float MAX_SPEED = 200.0f;   // Maximum upward speed
    const float MIN_SPEED = -200.0f;  // Maximum downward speed

    if (diff > margin || diff < -margin)
    {
        // Push up: Lift crane
        speed = (diff)*SPEED_SCALE;

        if (speed > MAX_SPEED)
        {
            speed = MAX_SPEED;
        }
        else if (speed < MIN_SPEED)
        {
            speed = MIN_SPEED;
        }
    }

    else
    {
        // Within margin: Stop crane
        speed = 0.0f;
    }
}