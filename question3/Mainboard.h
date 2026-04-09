#pragma once
#include <cstdint>
#include "MemoryModule.h"
#include "Keyboard.h"
#include "GraphicsAdapter.h"

class Mainboard
{
private:
    uint16_t Address_bus;
    uint8_t Data_bus;
    bool READ_ENABLE, WRITE_ENABLE;

    bool system_power;
    bool memory_active;
    bool gpu_writing;

    MemoryModule* ram_slot;
    Keyboard* key_slot;
    GraphicsAdapter* gpu_slot;

public:
    Mainboard();
    void connect_ram(MemoryModule* m);
    void connect_keyboard(Keyboard* k);
    void connect_gpu(GraphicsAdapter* g);

    uint8_t bus_read(uint16_t address);
    void bus_write(uint16_t address, uint8_t val);
    void clock_pulse();

    void killPower();
    bool is_powered();
    bool is_memory_active();
    bool is_gpu_active();
};