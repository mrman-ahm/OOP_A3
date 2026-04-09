#pragma once
#include <cstdint>
#include "RegisterBank.h"
#include "ALU.h"
#include "Mainboard.h"

struct SignalVector
{
    bool IS_VALID;
    bool USES_AEC;
    uint8_t AEC_OPERATION;
    bool IS_MEMORY_READ;
    bool IS_MEMORY_WRITE;
    bool IS_BRANCH;
    bool BRANCH_ON_ZERO;
    bool BRANCH_ON_NOT_ZERO;
    bool WRITES_TO_REGISTER;
    bool IS_4BIT_IMMEDIATE;
    bool IS_16BIT_IMMEDIATE;
};

class Processor
{
private:
    RegisterBank storage_bank;
    ALU aec;
    Mainboard* mainboard_ref;

    uint8_t cache_data[16];
    uint16_t cache_base_addr;
    bool cache_valid;

    SignalVector decode_matrix[256];
    double current_temp;

    void init_decode_matrix();
    uint8_t fetch_with_cache(uint16_t address);

public:
    Processor();
    void plug_in(Mainboard* m);
    bool step();

    RegisterBank& get_storage() { return storage_bank; }
    double get_temperature() { return current_temp; }
};