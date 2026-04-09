#pragma once
#include <cstdint>
#include "PhosphorusDisplay.h"

class GraphicsAdapter
{
private:
    PhosphorusDisplay* target_display;
public:
    GraphicsAdapter();
    void plug_in_display(PhosphorusDisplay* d);
    void CHAR_PAYLOAD(uint8_t val); 
    void INT_PAYLOAD(uint8_t val);  
};