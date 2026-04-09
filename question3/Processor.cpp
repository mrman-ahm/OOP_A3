#include "Processor.h"
#include <iostream>

using namespace std;

Processor::Processor()
{
    mainboard_ref = nullptr;
    current_temp = 25.0;
    cache_valid = false;
    cache_base_addr = 0;
    init_decode_matrix();
}

void Processor::plug_in(Mainboard* m)
{
    mainboard_ref = m;
}

uint8_t Processor::fetch_with_cache(uint16_t address)
{
    if (cache_valid && address >= cache_base_addr && address < cache_base_addr + 16)
        return cache_data[address - cache_base_addr];

    cache_base_addr = address & 0xFFF0;
    for (int i = 0; i < 16; i++)
        cache_data[i] = mainboard_ref->bus_read(cache_base_addr + i);

    cache_valid = true;
    return cache_data[address - cache_base_addr];
}

void Processor::init_decode_matrix()
{
    for (int i = 0; i < 256; i++)
        decode_matrix[i].IS_VALID = false;

    decode_matrix[0x00] = { true, false, 0x00, false, false, false, false, false, false, false, false };
    decode_matrix[0x01] = { true, true, 0x01, false, false, false, false, false, true, false, false };
    decode_matrix[0x02] = { true, true, 0x02, false, false, false, false, false, true, false, false };
    decode_matrix[0x03] = { true, true, 0x03, false, false, false, false, false, true, false, false };
    decode_matrix[0x04] = { true, true, 0x04, false, false, false, false, false, true, false, false };
    decode_matrix[0x0A] = { true, true, 0x0A, false, false, false, false, false, false, false, false };
    decode_matrix[0x10] = { true, false, 0x00, false, false, true, false, false, false, false, false };
    decode_matrix[0x11] = { true, false, 0x00, false, false, true, true, false, false, false, false };
    decode_matrix[0x12] = { true, false, 0x00, false, false, true, false, true, false, false, false };
    decode_matrix[0x1A] = { true, false, 0x00, false, false, false, false, false, true, true, false };
    decode_matrix[0x1B] = { true, false, 0x00, false, false, false, false, false, true, false, true };
    decode_matrix[0x20] = { true, false, 0x00, true, false, false, false, false, true, false, false };
    decode_matrix[0x21] = { true, false, 0x00, false, true, false, false, false, false, false, false };
}

bool Processor::step()
{
    uint8_t flags = storage_bank.readFLAGS();

    if (flags & RegisterBank::HF)
        return false;

    if (mainboard_ref && !mainboard_ref->is_powered())
        return false;

    uint16_t pc = storage_bank.readPC();
    uint8_t high = fetch_with_cache(pc);
    uint8_t low = fetch_with_cache(pc + 1);
    uint16_t ir = (high << 8) | low;

    if (ir == 0x0000)
        return false;

    storage_bank.writeIR(ir);
    storage_bank.writePC(pc + 2);

    SignalVector signals = decode_matrix[high];

    if (!signals.IS_VALID)
    {
        cout << "[HARDWARE FAULT] Invalid Instruction" << endl;
        return true;
    }

    uint16_t immediate_payload = 0;
    if (signals.IS_16BIT_IMMEDIATE)
    {
        uint16_t npc = storage_bank.readPC();
        immediate_payload = (fetch_with_cache(npc) << 8) | fetch_with_cache(npc + 1);
        storage_bank.writePC(npc + 2);
    }

    uint8_t dest = (ir >> 4) & 0x0F;
    uint8_t src = ir & 0x0F;
    uint16_t valA = storage_bank.readR(dest);
    uint16_t valB = storage_bank.readR(src);
    uint16_t result = 0;

    if (signals.USES_AEC)
    {
        result = aec.execute(signals.AEC_OPERATION, valA, valB, flags);
        storage_bank.writeFLAGS(flags);
    }
    else if (signals.IS_MEMORY_READ)
        result = mainboard_ref->bus_read(valB);
    else if (signals.IS_MEMORY_WRITE)
    {
        mainboard_ref->bus_write(valB, (uint8_t)valA);
        if (cache_valid && valB >= cache_base_addr && valB < cache_base_addr + 16)
            cache_valid = false;
    }
    else if (signals.IS_BRANCH)
    {
        bool take = true;
        if (signals.BRANCH_ON_ZERO && !(flags & RegisterBank::ZF)) take = false;
        if (signals.BRANCH_ON_NOT_ZERO && (flags & RegisterBank::ZF)) take = false;
        if (take) storage_bank.writePC(valB);
    }
    else if (signals.IS_4BIT_IMMEDIATE)
        result = src;
    else if (signals.IS_16BIT_IMMEDIATE)
        result = immediate_payload;

    if (signals.WRITES_TO_REGISTER)
        storage_bank.writeR(dest, result);

    current_temp += 0.05;

    if (current_temp >= 90.0)
    {
        cout << "[CRITICAL ERROR] Thermal threshold exceeded" << endl;
        flags |= RegisterBank::HF;
        storage_bank.writeFLAGS(flags);
        return false;
    }

    return true;
}