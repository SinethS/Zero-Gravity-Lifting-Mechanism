#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor.h"

#define PROFILE_UPDATE_FREQUENCY 125

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
// --- End Configuration ---

class ProfileController;

extern ProfileController *g_profileController;

class ProfileController
{
private:
    motor *_motor; // Pointer to the motor object we are controlling

    volatile int _current_rpm;
    volatile int _target_rpm;

    unsigned int _acceleration; // Acceleration in RPM per second
    int _rpm_step;              // The change in RPM per timer tick, calculated from acceleration



public:
    /**
     * @brief Constructor for the ProfileController.
     * @param motor_obj A pointer to the motor object to be controlled.
     * @param acceleration_rpm_s The desired acceleration in RPM/s. This determines the ramp steepness.
     */
    ProfileController(motor *motor_obj);

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