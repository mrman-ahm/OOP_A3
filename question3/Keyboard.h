#pragma once
#include <cstdint>

class Keyboard {
private:
    static const int MAX_CAPACITY = 512;

    char buffer[MAX_CAPACITY];
    int head, tail, count;

    void TRIGGER_INTERRUPT();

public:
    Keyboard();
    bool PENDING_INPUT();
    uint8_t READ_CHAR();
    uint8_t READ_INT();
};