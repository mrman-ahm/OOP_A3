#pragma once
#include <cstdint>

class MemoryModule
{
private:
    uint8_t storage[3840];
public:
    MemoryModule();
    uint8_t read_byte(uint16_t address);
    void write_byte(uint16_t address, uint8_t val);
    void flash_byte(uint16_t address, uint8_t val);
    void loadRawBinary(uint16_t startAddress, uint16_t word);
};