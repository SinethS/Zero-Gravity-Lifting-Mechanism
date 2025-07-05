#include "EEPROMManager.h"

bool EEPROMManager::store(const char *name, uint16_t val)
{
    int index = findEntryIndex(name);
    EEPROMEntry entry;
    entry.magic = MAGIC_TAG;
    strncpy(entry.name, name, ENTRY_NAME_LEN);
    entry.value = val;

    if (index < 0)
    {
        index = findFreeSlot();
        if (index < 0)
            return false;
    }

    eeprom_write_block(&entry, (void *)(EEPROM_START_ADDR + index * sizeof(EEPROMEntry)), sizeof(EEPROMEntry));
    return true;
}

bool EEPROMManager::read(const char *name, uint16_t *outVal)
{
    int index = findEntryIndex(name);
    if (index < 0)
        return false;

    EEPROMEntry entry;
    eeprom_read_block(&entry, (const void *)(EEPROM_START_ADDR + index * sizeof(EEPROMEntry)), sizeof(EEPROMEntry));

    if (entry.magic == MAGIC_TAG)
    {
        *outVal = entry.value;
        return true;
    }

    return false;
}

bool EEPROMManager::storeIfChanged(const char *name, uint16_t val)
{
    uint16_t currentVal;
    bool exists = read(name, &currentVal);

    if (!exists || currentVal != val)
    {
        return store(name, val);
    }
    // No write needed, value is the same
    return true;
}

int EEPROMManager::findEntryIndex(const char *name)
{
    EEPROMEntry entry;
    for (int i = 0; i < MAX_ENTRIES; i++)
    {
        eeprom_read_block(&entry, (const void *)(EEPROM_START_ADDR + i * sizeof(EEPROMEntry)), sizeof(EEPROMEntry));
        if (entry.magic == MAGIC_TAG && strncmp(entry.name, name, ENTRY_NAME_LEN) == 0)
        {
            return i;
        }
    }
    return -1;
}

int EEPROMManager::findFreeSlot()
{
    EEPROMEntry entry;
    for (int i = 0; i < MAX_ENTRIES; i++)
    {
        eeprom_read_block(&entry, (const void *)(EEPROM_START_ADDR + i * sizeof(EEPROMEntry)), sizeof(EEPROMEntry));
        if (entry.magic != MAGIC_TAG)
        {
            return i;
        }
    }
    return -1;
}
// End of EEPROMManager.cpp