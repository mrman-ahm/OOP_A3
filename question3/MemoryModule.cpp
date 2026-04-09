#include "MemoryModule.h"
#include <iostream>
using namespace std;

MemoryModule::MemoryModule()
{
    for (int i = 0; i < 3840; i++)
        storage[i] = 0x00;
}

uint8_t MemoryModule::read_byte(uint16_t address)
{
    if (address >= 0x0F00)
    {
        cout << "[HARDWARE FAULT] Segmentation Fault: Read out of bounds " << endl;
        return 0xFF;
    }
    return storage[address];
}

void MemoryModule::write_byte(uint16_t address, uint8_t val)
{
    if (address >= 0x0F00)
    {
        cout << "[HARDWARE FAULT] Segmentation Fault: Write out of bounds" << endl;
        return;
    }
    storage[address] = val;
}

void MemoryModule::flash_byte(uint16_t address, uint8_t val)
{
    if (address < 3840) storage[address] = val;
}

void MemoryModule::loadRawBinary(uint16_t startAddress, uint16_t word)
{
    flash_byte(startAddress, (word >> 8) & 0xFF);
    flash_byte(startAddress + 1, word & 0xFF);
}