#pragma once

class PhosphorusDisplay
{
private:
    char screen_buffer[16][32];
    int current_row, current_col;
public:
    PhosphorusDisplay();
    void buffer_char(char c);
    void FLUSH_OUTPUT();
};