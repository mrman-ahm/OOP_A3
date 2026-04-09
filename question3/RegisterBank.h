#pragma once
#include <cstdint>

class RegisterBank
{
private:
    uint16_t R[8];
    uint16_t PC;
    uint16_t IR;
    uint8_t  FLAGS;

public:
    RegisterBank();

    uint16_t readR(int index);
    void writeR(int index, uint16_t value);

    uint16_t readPC();
    void writePC(uint16_t value);

    uint16_t readIR();
    void writeIR(uint16_t value);

    uint8_t readFLAGS();
    void writeFLAGS(uint8_t value);

    static const uint8_t ZF = 0x01;
    static const uint8_t NF = 0x02;
    static const uint8_t PF = 0x04;
    static const uint8_t HF = 0x80;
};