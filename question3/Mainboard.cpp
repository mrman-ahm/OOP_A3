#include "Mainboard.h"

Mainboard::Mainboard()
{
    Address_bus = 0;
    Data_bus = 0;
    READ_ENABLE = false;
    WRITE_ENABLE = false;
    system_power = true;
    memory_active = false;
    gpu_writing = false;
    ram_slot = nullptr;
    key_slot = nullptr;
    gpu_slot = nullptr;
}

void Mainboard::connect_ram(MemoryModule* m) { ram_slot = m; }
void Mainboard::connect_keyboard(Keyboard* k) { key_slot = k; }
void Mainboard::connect_gpu(GraphicsAdapter* g) { gpu_slot = g; }
void Mainboard::clock_pulse() {}

void Mainboard::killPower() { system_power = false; }
bool Mainboard::is_powered() { return system_power; }
bool Mainboard::is_memory_active() { return memory_active; }
bool Mainboard::is_gpu_active() { return gpu_writing; }

uint8_t Mainboard::bus_read(uint16_t address)
{
    Address_bus = address;
    READ_ENABLE = true;

    memory_active = (Address_bus <= 0x0EFF);

    if (key_slot && Address_bus == 0x0FF0)
        Data_bus = key_slot->READ_CHAR();
    else if (key_slot && Address_bus == 0x0FF3)
        Data_bus = key_slot->READ_INT();
    else if (ram_slot && Address_bus <= 0x0EFF)
        Data_bus = ram_slot->read_byte(Address_bus);
    else
        Data_bus = 0x00;

    clock_pulse();
    READ_ENABLE = false;
    memory_active = false; 
    return Data_bus;
}

void Mainboard::bus_write(uint16_t address, uint8_t val)
{
    Address_bus = address;
    Data_bus = val;
    WRITE_ENABLE = true;

    memory_active = (Address_bus <= 0x0EFF);
    gpu_writing = (Address_bus == 0x0FF1 || Address_bus == 0x0FF2);

    if (gpu_slot && Address_bus == 0x0FF1)
        gpu_slot->CHAR_PAYLOAD(Data_bus);
    else if (gpu_slot && Address_bus == 0x0FF2)
        gpu_slot->INT_PAYLOAD(Data_bus);
    else if (ram_slot && Address_bus <= 0x0EFF)
        ram_slot->write_byte(Address_bus, Data_bus);

    clock_pulse();
    WRITE_ENABLE = false;
    memory_active = false; 
    gpu_writing = false;
}