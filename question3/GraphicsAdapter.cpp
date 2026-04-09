#include "GraphicsAdapter.h"

using namespace std;

GraphicsAdapter::GraphicsAdapter()
    { target_display = nullptr; }

void GraphicsAdapter::plug_in_display(PhosphorusDisplay* d)
    { target_display = d; }

void GraphicsAdapter::CHAR_PAYLOAD(uint8_t val)
{
    if (target_display != nullptr) target_display->buffer_char((char)val);
}

void GraphicsAdapter::INT_PAYLOAD(uint8_t val)
{
    if (target_display == nullptr) return;

    if (val == 0)
    {
        target_display->buffer_char('0');
        return;
    }

    char digits[4];
    int i = 0;
    uint8_t temp = val;

    while (temp > 0)
    {
        digits[i++] = (temp % 10) + '0';
        temp /= 10;
    }
    while (i > 0) target_display->buffer_char(digits[--i]); 
}
