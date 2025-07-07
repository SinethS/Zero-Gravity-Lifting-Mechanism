#ifndef TOUCHCONTROLLER_H
#define TOUCHCONTROLLER_H

#include "stdint.h"

class TouchController
{
private:
    float initial;      // Initial touch position/value
    float speed;        // Touch speed
    const float margin; // Constant margin value
    uint32_t error;     // Error value for PID control

public:
    // Constructor
    TouchController(float marginValue = 10000.0f);

    // Get initial value
    float getInitial() const;

    // Get speed value
    float getSpeed() const;

    float getMargin() const;

    uint32_t getError() const { return error; }

    // Update both initial and speed values
    void updateInitial(float newInitial);
    void updateSpeed(float ADC_value);
};

#endif // TOUCHCONTROLLER_H