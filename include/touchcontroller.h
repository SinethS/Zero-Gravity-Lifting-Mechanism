#ifndef TOUCHCONTROLLER_H
#define TOUCHCONTROLLER_H

class TouchController
{
private:
    float initial;      // Initial touch position/value
    float speed;        // Touch speed
    const float margin; // Constant margin value

public:
    // Constructor
    TouchController(float marginValue = 0.0f);

    // Get initial value
    float getInitial() const;

    // Get speed value
    float getSpeed() const;

    // Update both initial and speed values
    void updateInitial(float newInitial);
    void updateSpeed(float ADC_value);
};

#endif // TOUCHCONTROLLER_H