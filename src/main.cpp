#include <avr/io.h>
#include <util/delay.h>
#include "motor.h"
#include "ProfileController.h"

// =============================================================================
//               CONFIGURATION
// =============================================================================

// Define motor properties
#define MICROSTEPS 200

// Define the controller's acceleration. A lower value gives a slower, smoother ramp.
// A higher value gives a faster, more aggressive ramp.
#define ACCELERATION 300 // RPM per second

// Define the maximum speed for the continuous motion cycle.
#define MAX_RPM 300

// =============================================================================
//               OBJECT INSTANTIATION
// =============================================================================

// 1. Create the low-level motor object
motor my_motor(MICROSTEPS);

// 2. Create the high-level profile controller object.
ProfileController profile_controller(&my_motor, ACCELERATION);

// =============================================================================
//               MAIN APPLICATION
// =============================================================================

// We will use a simple enum to keep track of the motor's state in the cycle.
enum MotorState
{
    RAMPING_UP,
    RAMPING_DOWN
};

int main(void)
{
    // Initialize the controller. This handles all hardware setup.
    profile_controller.init();

    // Set the initial state for our cycle.
    MotorState current_state = RAMPING_UP;

    // Start the first movement: ramp up to MAX_RPM.
    profile_controller.run(MAX_RPM);

    // This is the main program loop.
    while (1)
    {
        // The core logic is to check if the motor profile is idle (i.e., it has reached its target RPM).
        // If it is idle, we command the next move in the cycle.

        if (profile_controller.is_idle())
        {

            // Check which state we just completed.
            if (current_state == RAMPING_UP)
            {
                // We have successfully reached +MAX_RPM.
                // Wait for a moment before reversing.
                _delay_ms(3000); // Hold at top speed for 1 second.

                // Now, command the motor to ramp down to the negative target.
                profile_controller.run(-MAX_RPM);

                // Update the state for the next check.
                current_state = RAMPING_DOWN;
            }
            else if (current_state == RAMPING_DOWN)
            {
                // We have successfully reached -MAX_RPM.
                // Wait for a moment before reversing again.
                _delay_ms(3000); // Hold at reverse speed for 1 second.

                // Now, command the motor to ramp up to the positive target.
                profile_controller.run(MAX_RPM);

                // Update the state to complete the cycle.
                current_state = RAMPING_UP;
            }
        }

        // --- IMPORTANT ---
        // Even while waiting, other tasks can be performed here.
        // The motor control is happening entirely in the background via interrupts.
        // This small delay prevents this loop from running at max CPU speed,
        // which is good practice.
        _delay_ms(10);
    }

    return 0; // This line will never be reached.
}