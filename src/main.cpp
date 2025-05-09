#include <avr/io.h>
#include <util/delay.h>
#include "HX711.h"
#include <stdio.h>
#include "UART.h"
#include "I2C.h"

// Kalman filter variables
float kalmanEstimate = 0.0;   // Current estimate
float kalmanError = 1.0;      // Error in estimate
float processNoise = 0.01;    // Process noise (tune this value)
float measurementNoise = 1.0; // Measurement noise (tune this value)
float kalmanGain = 0.0;       // Kalman gain

// Kalman filter function
float kalmanFilter(float measurement)
{
  // Update Kalman gain
  kalmanGain = kalmanError / (kalmanError + measurementNoise);

  // Update estimate
  kalmanEstimate = kalmanEstimate + kalmanGain * (measurement - kalmanEstimate);

  // Update error
  kalmanError = (1 - kalmanGain) * kalmanError + processNoise;

  return kalmanEstimate;
}

int main()
{
  // SET UP

  // Initialize HX711 object
  HX711 hx711(PE4, PE5); // PE4 = Data, PE5 = Clock

  // Set up serial communication
  UART uart(115200); // Initialize UART with baud rate 115200

  // Variables for calibration
  long referenceZero = 0;
  const int calibrationTime = 250; // 5 seconds (500 samples at 10ms intervals)
  long sum = 0;

  // Calibration: Take the first 5 seconds of values and calculate the average
  for (int i = 0; i < calibrationTime; i++)
  {
    uart.println("Calibrating..."); // Print calibration message
    long rawValue = hx711.read();
    sum += rawValue;
    _delay_ms(5); // 10ms delay
  }
  uart.println("Calibration complete!"); // Print completion message

  referenceZero = sum / calibrationTime; // Calculate the average

  // Main loop
  while (true)
  {
    // Read the raw value from the HX711
    long rawValue = hx711.read_average(10); // Read average of 10 samples
    uart.print("Raw Value: ");
    uart.println(rawValue); // Print the raw value

    // Apply Kalman filter to the raw value
    float filteredValue = kalmanFilter(rawValue);

    // Calculate the difference from the reference zero
    float difference = filteredValue - referenceZero;

    // Print the filtered difference
    uart.println(difference, 2); // Print the difference with 2 decimal places

    // Delay for 10 ms
    _delay_ms(10);
  }

  return 0;
}