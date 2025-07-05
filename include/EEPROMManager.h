#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

#include <avr/eeprom.h>
#include <stdint.h>
#include <string.h>

#define EEPROM_START_ADDR 0x00
#define MAX_ENTRIES 10
#define ENTRY_NAME_LEN 12
#define MAGIC_TAG 0xAB

struct EEPROMEntry
{
    uint8_t magic;
    char name[ENTRY_NAME_LEN];
    uint16_t value;
};

class EEPROMManager
{
public:
    bool store(const char *name, uint16_t val);
    bool read(const char *name, uint16_t *outVal);
    bool storeIfChanged(const char *name, uint16_t val);

private:
    int findEntryIndex(const char *name);
    int findFreeSlot();
};

#endif

/*
EXAMPLE USAGE OF EEPROMManager

EEPROMManager eepromManager;

int main()
{
    int motor_speed = 1234; // Example value to store
    uart.transmitString("EEPROM Manager Demo\n");

    uint16_t val;
    if (eepromManager.read("motor_speed", &val))
    {
        uart.transmitString("Restored motor_speed: ");
        uart.transmitNumber(val);
        uart.transmitString("\n");
    }
    else
    {
        uart.transmitString("No motor_speed found in EEPROM.\n");
    }

    // Try to write the same value(will skip write if no change) if (eepromManager.storeIfChanged("motor_speed", motor_speed))
    {
        uart.transmitString("motor_speed is stored or already up to date.\n");
    }

    while (1)
    {
        _delay_ms(500);
        if (eepromManager.read("motor_speed", &val))
        {
            uart.transmitString("Loop read motor_speed: ");
            uart.transmitNumber(val);
            uart.transmitString("\n");
        }

        _delay_ms(1000);   // Wait before next update
        motor_speed += 10; // Increment value for next iteration

        if (eepromManager.storeIfChanged("motor_speed", motor_speed))
        {
            uart.transmitString("motor_speed updated to: ");
            uart.transmitNumber(motor_speed);
            uart.transmitString("\n");
        }
    }
}
*/