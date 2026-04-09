#pragma once
#include <cstdint>

//Done

class ALU 
{
private:
    void ZF_update(uint16_t res, uint8_t& FLAGS);
    void CMP_Flag_Update(uint16_t op_A, uint16_t op_B, uint8_t& FLAGS);
public:
    uint16_t execute(uint8_t OPCODE, uint16_t op_A, uint16_t op_B, uint8_t&FLAGS);
};
