#include "ADS1232.h"

void ADS1232_Init() {
    DDRE |= (1 << PE5); // PE5 = SCLK → OUTPUT
    DDRE |= (1 << PE6); // PE6 = POWER → OUTPUT
    DDRE &= ~(1 << PE4); // PE4 = DOUT → INPUT

    // ----------- Power ON ADS1232 ------------------------
    PORTE |= (1 << PE6);  // Set PE6 HIGH to power the ADS1232

    // ----------- Wait for Power Stabilization ------------
    _delay_ms(100);  // Let the chip power up

    // ----------- Optional: Generate a few clock pulses ---
    for (int i = 0; i < 5; i++) {
        PORTE |= (1 << PE5);  // SCLK HIGH
        _delay_us(10);
        PORTE &= ~(1 << PE5); // SCLK LOW
        _delay_us(10);
    }

}

// Kalman filter variables
float q = 0.1; // Process noise covariance
float r = 0.1; // Measurement noise covariance
float x_hat = 0; // Estimated value
float p = 1; // Estimation error covariance
float k; // Kalman gain

uint32_t ADS1232_Read() {
  uint32_t value = 0;

  // Wait for the data to be ready
  while (PINE & (1 << PE4));

  long data = 0;
  for (int i = 0; i < 24; i++) {
    PORTE |= (1 << PE5);  // SCLK HIGH
    _delay_us(1);

    data <<= 1;
    if (PINE & (1 << PE4)) {
      data |= 1;
    }

    PORTE &= ~(1 << PE5); // SCLK LOW
    _delay_us(1);
  }

  // Kalman filter update
  // Prediction update
  p += q;

  // Measurement update
  k = p / (p + r);
  x_hat += k * (data - x_hat);
  p *= (1 - k);

  // Return the filtered value
  return (uint32_t)x_hat;
}

void ADS1232_StartConversion() {
  // Start a new conversion by toggling the SCLK pin
  PORTE |= (1 << PE5);  // Set SCLK HIGH
  _delay_us(1);         // Small delay
  PORTE &= ~(1 << PE5); // Set SCLK LOW
} // Ensure this closing brace is correctly placed

void ADS1232_SetGain(uint8_t gain) {
  // Set gain for the ADS1232 (assuming gain is 1, 2, 64, or 128)
  // This function can be expanded based on the specific implementation
  // For now, we will just print the gain value for demonstration
  // In a real implementation, you would configure the gain register here
  if (gain == 1 || gain == 2 || gain == 64 || gain == 128) {
    // Set gain (implementation depends on your specific setup)
  }
}

uint32_t ADS1232_GetAverage(int samples) {
  uint32_t total = 0;
  for (int i = 0; i < samples; i++) {
    total += ADS1232_Read();
  }
  return total / samples;
}
