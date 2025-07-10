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
    // Initialize the low-level motor driver
    // _motor->initMotor();

    // Set up our high-level profile timer
    // init_profile_timer();

    // Set the global pointer to this instance so the ISR can find it
    g_profileController = this;

    // Enable global interrupts
    // sei();
}

// void ProfileController::init_profile_timer()
// {
//     // Configure Timer4 on the ATmega2560 for our periodic interrupt.

//     // 1. Reset timer control registers for a clean setup.
//     TCCR4A = 0;
//     TCCR4B = 0;

//     // 2. Set Waveform Generation Mode to CTC (Clear Timer on Compare Match).
//     // The timer will count up to OCR4A and then reset, triggering an interrupt.
//     TCCR4B |= (1 << WGM42);

//     // 3. Set the prescaler. A prescaler of 64 is a good choice for this frequency.
//     // CS41=1, CS40=1 -> Prescaler = 64
//     TCCR4B |= (1 << CS41) | (1 << CS40);

//     // 4. Calculate the compare match value (OCR4A) for our desired frequency.
//     // Formula: OCR_value = (F_CPU / (prescaler * frequency)) - 1
//     // OCR4A = (16,000,000 / (64 * 100)) - 1 = 2500 - 1 = 2499
//     OCR4A = (F_CPU / (64UL * PROFILE_UPDATE_FREQUENCY)) - 1;

//     // 5. Enable the Timer4 Compare A match interrupt.
//     TIMSK4 |= (1 << OCIE4A);
// }

void ProfileController::run(int target_rpm)
{
    // Simply update the target. The ISR will handle the rest.
    _target_rpm = target_rpm;
    update(); // Call update immediately to set the initial speed
}

void ProfileController::update()
{
    // This is the core logic, executed at a fixed frequency by the ISR.

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