#include "ProfileController.h"

// Define the global pointer. It will be assigned in your main application.
ProfileController *g_profileController = nullptr;

/**
 * @brief The Interrupt Service Routine for Timer4 Compare Match A.
 * This is the "heartbeat" of our controller, firing at PROFILE_UPDATE_FREQUENCY.
 */
// ISR(TIMER4_COMPA_vect)
// {
//     // If the global pointer is valid, call the object's update method.
//     if (g_profileController)
//     {
//         g_profileController->update();
//     }
// }

ProfileController::ProfileController(motor *motor_obj)
{
    _motor = motor_obj;
    _acceleration = 300; // Default acceleration in RPM/s

    _current_rpm = 0;
    _target_rpm = 0;

    // Pre-calculate the RPM change per tick to avoid division in the ISR.
    _rpm_step = _acceleration / PROFILE_UPDATE_FREQUENCY;
    // Ensure the step is at least 1 if acceleration is non-zero, due to integer division.
    if (_rpm_step == 0 && _acceleration > 0)
    {
        _rpm_step = 1;
    }
}

void ProfileController::init()
{
 
    g_profileController = this;
}


void ProfileController::run(int target_rpm)
{
    _target_rpm = target_rpm;
    update(); // Call update immediately to set the initial speed
}

void ProfileController::update()
{

    // --- Ramping Logic ---
    if (_current_rpm < _target_rpm)
    {
        // We need to accelerate
        _current_rpm += _rpm_step;
        // Prevent overshoot
        if (_current_rpm > _target_rpm)
        {
            _current_rpm = _target_rpm;
        }
    }
    else if (_current_rpm > _target_rpm)
    {
        // We need to decelerate
        _current_rpm -= _rpm_step;
        // Prevent overshoot
        if (_current_rpm < _target_rpm)
        {
            _current_rpm = _target_rpm;
        }
    }

    // Send the new, updated speed to the low-level motor controller.
    // The speedcontrol function handles direction (via the sign) and PWM output.
    _motor->speedcontrol(_current_rpm);
}

void ProfileController::emergency_stop()
{
    _target_rpm = 0;
    _current_rpm = 0;
    _motor->stopMotor(); // Use the motor's hard stop function
}

int ProfileController::get_current_rpm()
{
    return _current_rpm;
}

bool ProfileController::is_idle()
{
    // The profile is considered complete (idle) when the current speed matches the target speed.
    return _current_rpm == _target_rpm;
}