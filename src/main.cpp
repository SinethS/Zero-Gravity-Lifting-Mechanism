#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "motor.h"
#include "UART.h"
#include "IO.h"
#include "timemillis.h"
#include "I2C.h"
#include "linearControl.h"
#include "ADS1232.h"
#include "touchcontroller.h"

// defines

// defines end

// variable declarations

TouchController touchController(2000); // Initialize TouchController
motor stepper(1600);                   // Initialize motor with 1600 microsteps
UART uart;                             // Initialize UART
IO io;
LinearControl controller; // Initialize LinearControl

// varible declarations end

// function declarations

// ISR(INT4_vect) {
//     hx711.detach_interrupt();  // Detach interrupt to prevent re-entrance
//     bool hx711_data_ready = hx711.is_ready();  // Check if HX711 data is ready
//     if(hx711_data_ready) {
//         hx711.read();  // Read raw value from HX711
//     }

//     hx711.attach_interrupt();  // Detach interrupt to prevent re-entrance
// }

ISR(TIMER5_COMPA_vect)
{
    stepper.stopMotor(); // Stop motor on compare match
    uart.println("Motor stopped");
}

ISR(PCINT1_vect)
{
    io.detachINTERUPT_PCINT1(); // Detach interrupt for Port K
    int x = io.buttonUpdate();  // Update button state
    // create and put update display or get button input display
    // uart.transmitNumber(x);  // Send button state over UART
    io.attacthINTERUPT_PCINT1(); // Reattach interrupt for Port K
}

// function declarations end

// Map function similar to Arduino's map()
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
    // Initialize peripherals
    ADS1232_Init(); // Initialize ADS1232

    uart.transmitString("hello world!"); // Send message over UART

    timer2_ctc_100hz_init();            // Initialize Timer2 for 100 Hz
    stepper.initMotor();                // Initialize motor
    uart.println("Motor initialized");  // Send message over UART
    millis_init();                      // Initialize millis
    uart.println("Millis initialized"); // Send message over UART
    io.initIO();                        // Initialize IO
    uart.println("IO initialized");     // Send message over UART
    // Initialize HX711 object
    uart.println("HX711 initialized");         // Send message over UART
    controller.begin();                        // Initialize LinearControl
    uart.println("LinearControl initialized"); // Send message over UART

    // stepper.speedcontrol(0);
    // stepper.ENmotor();
    controller.start_conversion(); // Start ADC conversion

    // DDRE |= (1 << PE5); // PE5 = SCLK → OUTPUT
    // DDRE |= (1 << PE6); // PE6 = POWER → OUTPUT
    // DDRE &= ~(1 << PE4); // PE4 = DOUT → INPUT

    // // ----------- Power ON ADS1232 ------------------------
    // PORTE |= (1 << PE6);  // Set PE6 HIGH to power the ADS1232

    // // ----------- Wait for Power Stabilization ------------
    // _delay_ms(100);  // Let the chip power up

    // // ----------- Optional: Generate a few clock pulses ---
    // for (int i = 0; i < 5; i++) {
    //     PORTE |= (1 << PE5);  // SCLK HIGH
    //     _delay_us(10);
    //     PORTE &= ~(1 << PE5); // SCLK LOW
    //     _delay_us(10);
    // }

    ADS1232_GetAverage(5000); // Update initial touch value

    touchController.updateInitial(ADS1232_GetAverage(100)); // Update initial touch value

    while (1)
    {
        // char buffer[50]; // Buffer for UART transmission
        // // Print result
        // sprintf(buffer, "Data: %ld\n", touchController.getSpeed()); // Format
        // uart.transmitString(buffer);

        // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
        if (every_5_seconds())
        {
            touchController.updateInitial(ADS1232_GetAverage(50)); // Update initial touch value
        }

        if (get_flag())
        {                 // Check if loop flag is set
            clear_flag(); // Clear loop flag
            // uart.println("Looping...");  // Send message over UART

            long data = ADS1232_Read(); // Read data from ADS1232

            // Update touch controller with new ADC value
            touchController.updateSpeed(data);
            // stepper.speedcontrol(touchController.getSpeed()); // Set motor speed based on touch controller

            // stepper.speedcontrol(200);

            uart.println(touchController.getSpeed()); // Send message over UART
        }
    }
}
