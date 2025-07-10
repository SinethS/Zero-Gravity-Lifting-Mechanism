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
