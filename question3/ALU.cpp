#include "ALU.h"
#include "RegisterBank.h" 

//Done

void ALU::ZF_update(uint16_t res, uint8_t& FLAGS) 
{
    if (res == 0x0000) FLAGS |= RegisterBank::ZF;
    else FLAGS &= ~RegisterBank::ZF;
}

void ALU::CMP_Flag_Update(uint16_t op_A, uint16_t op_B, uint8_t& FLAGS) 
{
    //clear all
    FLAGS &= ~(RegisterBank::ZF | RegisterBank::NF | RegisterBank::PF);
    //set respective flag
    if      (op_A == op_B) FLAGS |= RegisterBank::ZF;
    else if (op_A  < op_B) FLAGS |= RegisterBank::NF;
    else if (op_A  > op_B) FLAGS |= RegisterBank::PF;
}

uint16_t ALU::execute(uint8_t OPCODE, uint16_t op_A, uint16_t op_B, uint8_t& FLAGS) 
{
    uint16_t res = 0;
    switch (OPCODE) 
    {
        case 0x01:
            res = op_A + op_B;
            ZF_update(res, FLAGS);
            break;

        case 0x02: 
            res = op_A - op_B;
            ZF_update(res, FLAGS);
            break;

        case 0x03: 
            res = op_A * op_B;
            ZF_update(res, FLAGS);
            break;

        case 0x04: 
            if (op_B == 0) res = 0x0000;
            else res = op_A / op_B;
            ZF_update(res, FLAGS);
            break;

        case 0x0A:
            CMP_Flag_Update(op_A, op_B, FLAGS);
            return 0; 

        default:
             break;
    }
    return res;
}

