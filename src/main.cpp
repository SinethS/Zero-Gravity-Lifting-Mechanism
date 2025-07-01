// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <util/delay.h>
// #include <stdio.h>
// #include <stdint.h>
// #include "motor.h"
// #include "UART.h"
// #include "IO.h"
// #include "timemillis.h"
// #include "I2C.h"
// #include "linearControl.h"
// #include "ADS1232.h"
// #include <avr/eeprom.h>

// // defines

// // defines end

// // variable declarations

// motor stepper(1600);  // Initialize motor with 1600 microsteps
// UART uart;  // Initialize UART
// IO io;
// LinearControl controller;  // Initialize LinearControl

// // varible declarations end

// // function declarations

// // ISR(INT4_vect) {
// //     hx711.detach_interrupt();  // Detach interrupt to prevent re-entrance
// //     bool hx711_data_ready = hx711.is_ready();  // Check if HX711 data is ready
// //     if(hx711_data_ready) {
// //         hx711.read();  // Read raw value from HX711
// //     }

// //     hx711.attach_interrupt();  // Detach interrupt to prevent re-entrance
// // }

// ISR(TIMER5_COMPA_vect) {
//     stepper.stopMotor();  // Stop motor on compare match
//     uart.println("Motor stopped");
// }

// ISR(PCINT1_vect){
//     io.detachINTERUPT_PCINT1();  // Detach interrupt for Port K
//     int x = io.buttonUpdate();  // Update button state
//     // create and put update display or get button input display
//     // uart.transmitNumber(x);  // Send button state over UART
//     io.attacthINTERUPT_PCINT1();  // Reattach interrupt for Port K
// }

// // function declarations end

// // Map function similar to Arduino's map()
// long map(long x, long in_min, long in_max, long out_min, long out_max) {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }

// int main(void) {
//     // Initialize peripherals
//     ADS1232_Init();  // Initialize ADS1232

//     uart.transmitString("hello world!");  // Send message over UART

//     timer2_ctc_100hz_init();  // Initialize Timer2 for 100 Hz
//     stepper.initMotor();  // Initialize motor
//     uart.println("Motor initialized");  // Send message over UART
//     millis_init();  // Initialize millis
//     uart.println("Millis initialized");  // Send message over UART
//     io.initIO();  // Initialize IO
//     uart.println("IO initialized");  // Send message over UART
//     // Initialize HX711 object
//     uart.println("HX711 initialized");  // Send message over UART
//     controller.begin();  // Initialize LinearControl
//     uart.println("LinearControl initialized");  // Send message over UART

//     // stepper.speedcontrol(0);
//     // stepper.ENmotor();
//     controller.start_conversion();  // Start ADC conversion

//     int prv_speed = 0;  // Previous speed

//     // DDRE |= (1 << PE5); // PE5 = SCLK → OUTPUT
//     // DDRE |= (1 << PE6); // PE6 = POWER → OUTPUT
//     // DDRE &= ~(1 << PE4); // PE4 = DOUT → INPUT

//     // // ----------- Power ON ADS1232 ------------------------
//     // PORTE |= (1 << PE6);  // Set PE6 HIGH to power the ADS1232

//     // // ----------- Wait for Power Stabilization ------------
//     // _delay_ms(100);  // Let the chip power up

//     // // ----------- Optional: Generate a few clock pulses ---
//     // for (int i = 0; i < 5; i++) {
//     //     PORTE |= (1 << PE5);  // SCLK HIGH
//     //     _delay_us(10);
//     //     PORTE &= ~(1 << PE5); // SCLK LOW
//     //     _delay_us(10);
//     // }

//     while (1) {
//         // Loop forever — frequency generation is hardware-driven set by Timer2 (125Hz)
//         if(get_flag()) {  // Check if loop flag is set
//             clear_flag();  // Clear loop flag
//             // uart.println("Looping...");  // Send message over UART
//             char buffer[50];

//             long data = ADS1232_GetAverage(10);  // Read data from ADS1232

//         // while (PINE & (1 << PE4));

//         // long data = 0;
//         // for (int i = 0; i < 24; i++) {
//         //     PORTE |= (1 << PE5);  // SCLK HIGH
//         //     _delay_us(1);

//         //     data <<= 1;
//         //     if (PINE & (1 << PE4)) {
//         //     data |= 1;
//         //     }

//         //     PORTE &= ~(1 << PE5); // SCLK LOW
//         //     _delay_us(1);
//         // }

//         // Sign extend the 24-bit data

//             // Print result
//             sprintf(buffer, "Data: %ld\n", data);  // Format
//             uart.transmitString(buffer);  // Send data over UART

//             // // sprintf(buffer, "Time: %lu ms\n", millis());  // Get current time in milliseconds
//             // // uart.transmitString(buffer);  // Send time over UART
//             // float x = controller.get_filtered();
//             // sprintf(buffer, "Filtered Value: %.2f\n", x);  // Format filtered value
//             // uart.transmitString(buffer);  // Send filtered value over UART
//             // _delay_ms(100);  // Delay for 50 ms

//             // x = pow((x-10)/830, 0.5)*1000
//             // // x = x/1024*1000;  // Scale the filtered value
//             // int speed = (int)x;  // Convert to integer

//             // speed = map(speed, 0, 1000, -150, 150);  // Map the speed value to a range

//             // if(abs(speed - prv_speed) < 5) {  // Check if speed change is significant
//             //     speed = prv_speed;  // Use previous speed if change is small
//             // }

//             // prv_speed = speed;  // Update previous speed
//             // stepper.speedcontrol(speed);  // Control motor speed based on filtered value
//             // sprintf(buffer, "out rpm: %d, %.2f\n", speed, x);  // Format output string
//             // uart.transmitString(buffer);  // Send filtered value over UART
//             // sprintf(buffer, "Raw Value: %ld\n", hx711.get_raw_value());  // Get raw value from HX711
//             // uart.transmitString(buffer);  // Send raw value over UART
//             // sprintf(buffer, "Filtered Value: %.2f\n", controller.get_filtered());  // Get filtered value from LinearControl
//             // uart.transmitString(buffer);  // Send filtered value over UART

//             // loop code end
//         }
//     }
// }

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "UART.h"
#include "timemillis.h"

UART uart;
uint16_t EEMEM savedValueEEPROM; // EEPROM variable

void saveToEEPROM(uint16_t val)
{
    eeprom_write_word(&savedValueEEPROM, val);
    uart.transmitString("Stored value: ");
    uart.transmitNumber(val);
    uart.transmitString(" | ");
    char buf[32];
    sprintf(buf, "EEPROM address: %u", (unsigned)&savedValueEEPROM);
    uart.transmitString(buf);
    uart.transmitString("\n");
}

uint16_t readFromEEPROM()
{
    uint16_t val = eeprom_read_word(&savedValueEEPROM);
    uart.transmitString("Retrieved value: ");
    uart.transmitNumber(val);
    uart.transmitString("\n");
    return val;
}

int main(void)
{
    millis_init(); // Initialize timer
    uart.transmitString("EEPROM Overwrite Test\n");

    unsigned long startTime = millis();
    bool firstWriteDone = false;
    bool secondWriteDone = false;

    while (1)
    {
        unsigned long currentTime = millis();

        // if (!firstWriteDone && currentTime - startTime >= 3000) {
        //     saveToEEPROM(1234);     // First value
        //     readFromEEPROM();
        //     firstWriteDone = true;
        // }

        // if (!secondWriteDone && currentTime - startTime >= 8000) {
        //     saveToEEPROM(1000);     // Overwrite value
        //     readFromEEPROM();
        //     secondWriteDone = true;
        // }

        // Periodically read the value every 3s
        static unsigned long lastRead = 0;
        if (currentTime - lastRead >= 3000)
        {
            lastRead = currentTime;
            readFromEEPROM();
        }
    }
}

// #include "UART.h"

// UART uart;

// int main(void)
// {
//     uart.transmitString("Hello from UART!\n");

//     while (1)
//     {
//         uart.println("Testing Serial...");
//         _delay_ms(1000);
//     }
// }
