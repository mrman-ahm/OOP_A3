#include "RegisterBank.h"

RegisterBank::RegisterBank()
{
    for (int i = 0; i < 8; i++) R[i] = 0;
    PC = 0;
    IR = 0;
    FLAGS = 0;
}

uint16_t RegisterBank::readR(int index)
{
    if (index >= 0 && index < 8)
        return R[index];
    return 0;
}

void RegisterBank::writeR(int index, uint16_t value)
{
    if (index >= 0 && index < 8)
        R[index] = value;
}

uint16_t RegisterBank::readPC() { return PC; }
void RegisterBank::writePC(uint16_t value) { PC = value; }

uint16_t RegisterBank::readIR() { return IR; }
void RegisterBank::writeIR(uint16_t value) { IR = value; }

uint8_t RegisterBank::readFLAGS()
{
    return FLAGS;
}

void RegisterBank::writeFLAGS(uint8_t value)
{
    if (FLAGS & HF) FLAGS = value | HF;
    else FLAGS = value;
}