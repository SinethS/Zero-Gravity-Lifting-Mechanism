#include "ADS1232.h"

ADS1232::ADS1232(volatile uint8_t *port, volatile uint8_t *ddr, volatile uint8_t *pin,
                 uint8_t sclk_pin, uint8_t dout_pin, uint8_t power_pin)
    : _port(port), _ddr(ddr), _pin(pin), _sclk(sclk_pin), _dout(dout_pin), _power(power_pin) {}

void ADS1232::init()
{
  *_ddr |= (1 << _sclk) | (1 << _power); // SCLK and POWER as output
  *_ddr &= ~(1 << _dout);                // DOUT as input

  *_port |= (1 << _power); // Power ON
  _delay_ms(100);          // Wait for stabilization

  for (int i = 0; i < 5; i++)
    pulseClock();

  detachInterrupt(); // Ensure interrupt is detached before reading
}

void ADS1232::pulseClock()
{
  *_port |= (1 << _sclk);
  // _delay_us(10);
  *_port &= ~(1 << _sclk);
  // _delay_us(10);
}

void ADS1232::attachInterrupt()
{
  EICRB |= (1 << ISC41); // ISC41:1, ISC40:0 => Falling edge on INT4
  EICRB &= ~(1 << ISC40);
  EIFR |= (1 << INTF4); // Clear any pending INT4 interrupt
  EIMSK |= (1 << INT4); // Enable INT4
}

void ADS1232::detachInterrupt()
{
  EIMSK &= ~(1 << INT4); // Disable INT4
}

bool ADS1232::dataReady()
{
  return !(*_pin & (1 << _dout)); // Returns true if DOUT is LOW (i.e., data ready)
}

uint32_t ADS1232::read()
{
  while (*_pin & (1 << _dout))
    ; // Wait for data ready

  long data = 0;
  for (int i = 0; i < 24; i++)
  {
    *_port |= (1 << _sclk);
    // _delay_us(1);

    data <<= 1;
    if (*_pin & (1 << _dout))
      data |= 1;

    *_port &= ~(1 << _sclk);
    // _delay_us(1);
  }

  if (data & 0x800000)
    data |= 0xFF000000; // Sign extend

  pulseClock(); // Extra clock

  // Kalman Filter
  p += q;
  k = p / (p + r);
  x_hat += k * (data - x_hat);
  p *= (1 - k);

  return (uint32_t)x_hat;
}

void ADS1232::startConversion()
{
  *_port |= (1 << _sclk);
  // _delay_us(1);
  *_port &= ~(1 << _sclk);
}

void ADS1232::setGain(uint8_t gain)
{
  if (gain == 1 || gain == 2 || gain == 64 || gain == 128)
  {
    // Not implemented: gain setting would depend on hardware wiring (e.g., gain pins)
  }
}

uint32_t ADS1232::getAverage(uint8_t samples)
{
  detachInterrupt(); // Ensure interrupt is detached before reading
  uint32_t sum = 0;
  for (uint8_t i = 0; i < samples; i++)
  {
    sum += read();
  }
  return sum / samples;
  attachInterrupt(); // Reattach interrupt after reading
}

void ADS1232::calibrate()
{
  uint32_t sum = 0;
  for (uint8_t i = 0; i < 100; i++)
  {
    sum += read();
  }
  offset = sum / 100;
}

void ADS1232::CalcScale(float known_weight)
{  
  uint32_t average = getAverage(100); // Get average reading
  scale = (average - offset) / known_weight;
}

float ADS1232::Weight()
{
  weight = float(x_hat - offset) / scale;
  return weight;
}