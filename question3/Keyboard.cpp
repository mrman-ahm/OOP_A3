#include "Keyboard.h"
#include <iostream>

using namespace std;

Keyboard::Keyboard()
{
    head = 0; tail = 0; count = 0;
    for (int i = 0; i < MAX_CAPACITY; i++) buffer[i] = 0;
}

void Keyboard::TRIGGER_INTERRUPT()
{
    cout << endl << "[Hardware Interrupt] Awaiting Keyboard Input: ";

    char input_line[256];
    cin.getline(input_line, 256);

    for (int i = 0; input_line[i] != '\0'; i++)
    {
        if (count < MAX_CAPACITY)
        {
            buffer[tail] = input_line[i];
            tail = (tail + 1) % MAX_CAPACITY;
            count++;
        }
    }

    if (count < MAX_CAPACITY)
    {
        buffer[tail] = ' ';
        tail = (tail + 1) % MAX_CAPACITY;
        count++;
    }
}

bool Keyboard::PENDING_INPUT()
{
    for (int i = 0, h = head; i < count; i++) {
        if (buffer[h] != ' ' && buffer[h] != '\n' && buffer[h] != '\t') return true;
        h = (h + 1) % MAX_CAPACITY;
    }
    return false;
}

uint8_t Keyboard::READ_CHAR()
{
    while (!PENDING_INPUT()) TRIGGER_INTERRUPT();

    while (buffer[head] == ' ' || buffer[head] == '\n' || buffer[head] == '\t') {
        head = (head + 1) % MAX_CAPACITY;
        count--;
    }

    uint8_t val = (uint8_t)buffer[head];
    head = (head + 1) % MAX_CAPACITY;
    count--;
    return val;
}

uint8_t Keyboard::READ_INT()
{
    while (!PENDING_INPUT()) TRIGGER_INTERRUPT();

    uint32_t accumulator = 0;

    while (count > 0 && (buffer[head] < '0' || buffer[head] > '9')) {
        head = (head + 1) % MAX_CAPACITY;
        count--;
    }

    while (count > 0 && (buffer[head] >= '0' && buffer[head] <= '9'))
    {
        uint8_t digit = buffer[head] - '0';
        accumulator = (accumulator * 10) + digit;

        head = (head + 1) % MAX_CAPACITY;
        count--;
    }

    return (uint8_t)accumulator;
}