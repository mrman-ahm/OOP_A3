#include "PhosphorusDisplay.h"
#include <iostream>

using namespace std;

PhosphorusDisplay::PhosphorusDisplay()
{
    current_row = 0;
    current_col = 0;
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 32; j++)
            screen_buffer[i][j] = ' ';
}

void PhosphorusDisplay::buffer_char(char c)
{
    if (c == '\n')
    {
        current_row++;
        current_col = 0;
    }
    else
    {
        if (current_row < 16)
        {
            screen_buffer[current_row][current_col] = c;
            current_col++;
            if (current_col >= 32)
            {
                current_col = 0;
                current_row++;
            }
        }
    }
}

void PhosphorusDisplay::FLUSH_OUTPUT()
{
    cout << endl << "+--------------------------------+" << endl;
    for (int i = 0; i < 16; i++)
    {
        cout << "|";
        for (int j = 0; j < 32; j++) cout << screen_buffer[i][j];
        cout << "|" << endl;
    }
    cout << "+--------------------------------+" << endl;
}