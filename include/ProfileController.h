#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor.h"

// --- Configuration ---
// The frequency of the profile update timer in Hz.
// 50-100Hz is a good range for smooth acceleration.
#define PROFILE_UPDATE_FREQUENCY 100
// The system clock frequency. Must be defined for timer calculations.
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
// --- End Configuration ---

// Forward declaration of the class
class ProfileController;

// A global pointer to the single instance of the controller.
// This is a standard pattern required for C-style ISRs to call C++ class methods.
extern ProfileController *g_profileController;

class ProfileController
{
private:
    motor *_motor; // Pointer to the motor object we are controlling

    // Volatile is crucial for variables shared between main code and an ISR
    volatile int _current_rpm;
    volatile int _target_rpm;

    unsigned int _acceleration; // Acceleration in RPM per second
    int _rpm_step;              // The change in RPM per timer tick, calculated from acceleration

    // Private helper to initialize the hardware timer for profile updates
    void init_profile_timer();

public:
    /**
     * @brief Constructor for the ProfileController.
     * @param motor_obj A pointer to the motor object to be controlled.
     * @param acceleration_rpm_s The desired acceleration in RPM/s. This determines the ramp steepness.
     */
    ProfileController(motor *motor_obj, unsigned int acceleration_rpm_s);

    /**
     * @brief Initializes the motor and the profile controller's internal timer. Must be called in setup.
     */
    void init();

    /**
     * @brief The primary function to set a new target speed. The controller handles the ramp.
     * @param target_rpm The desired final RPM. A negative value indicates reverse direction.
     */
    void run(int target_rpm);

    /**
     * @brief This method is called by the timer ISR to update the motor speed. Do not call this directly.
     * It contains the core logic for the trapezoidal (ramp) profile.
     */
    void update();

    /**
     * @brief Immediately stops the motor, bypassing the deceleration ramp. Disables the motor.
     */
    void emergency_stop();

    /**
     * @brief Gets the current instantaneous RPM of the motor's profile.
     * @return The current RPM value being sent to the motor driver.
     */
    int get_current_rpm();

    /**
     * @brief Checks if the motor has reached its target RPM and the profile is complete.
     * @return True if current RPM == target RPM, false otherwise.
     */
    bool is_idle();
};

#endif // PROFILECONTROLLER_H