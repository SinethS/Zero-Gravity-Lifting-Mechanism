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
    return speed;
}

// Update both initial and speed values implementation
void TouchController::updateInitial(float newInitial)
{
    initial = newInitial;
}

void TouchController::updateSpeed(float ADC_value)
{
    int diff = ADC_value - initial; // Calculate difference from initial value

    if (diff > margin)
    {
        speed = speed++; // Scale up the speed for positive difference
    }
    else if (diff < -margin)
    {
        speed = speed--; // Scale down the speed for negative difference
    }
}
// If the difference is within the margin, speed remains unchanged