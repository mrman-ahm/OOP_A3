#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;


// ================================================================
// RegisterBank
// ================================================================

// holds all 11 storage cells of the ONYX-16 processor
// only accessible through read/write interface, nothing touches these directly
class RegisterBank
{
private:
    unsigned short R[8];     // 8 general purpose 16-bit registers R0-R7
    unsigned short PC;       // program counter - address of next instruction
    unsigned short IR;       // instruction register - currently executing instruction
    unsigned char  FLAGS;    // 8-bit status latch: ZF, NF, PF, HF

public:
    RegisterBank();

    unsigned short readR(int index);
    void           writeR(int index, unsigned short value);

    unsigned short readPC();
    void           writePC(unsigned short value);

    unsigned short readIR();
    void           writeIR(unsigned short value);

    unsigned char  readFLAGS();
    void           writeFLAGS(unsigned char value);

    // flag bitmasks as per spec
    static const unsigned char ZF = 0x01; // zero flag - equal or result was 0
    static const unsigned char NF = 0x02; // negative flag - left < right
    static const unsigned char PF = 0x04; // positive flag - left > right
    static const unsigned char HF = 0x80; // halt flag - thermal shutdown, never cleared
};

RegisterBank::RegisterBank()
{
    for (int i = 0; i < 8; i++) R[i] = 0; // all regs start at 0x0000
    PC = 0;
    IR = 0;
    FLAGS = 0;
}

unsigned short RegisterBank::readR(int index)
{
    if (index >= 0 && index < 8) return R[index];
    return 0;
}

void RegisterBank::writeR(int index, unsigned short value)
{
    if (index >= 0 && index < 8) R[index] = value;
}

unsigned short RegisterBank::readPC() { return PC; }
void           RegisterBank::writePC(unsigned short value) { PC = value; }

unsigned short RegisterBank::readIR() { return IR; }
void           RegisterBank::writeIR(unsigned short value) { IR = value; }

unsigned char  RegisterBank::readFLAGS() { return FLAGS; }

void RegisterBank::writeFLAGS(unsigned char value)
{
    if (FLAGS & HF) FLAGS = value | HF; // HF can never be cleared once set
    else            FLAGS = value;
}


// ================================================================
// ALU
// ================================================================

// stateless arithmetic execution core - no memory of past operations
// takes opcode + two operands, updates FLAGS directly, returns result
class ALU
{
private:
    void ZF_update(unsigned short res, unsigned char& FLAGS);                              // update zero flag after arithmetic
    void CMP_Flag_Update(unsigned short op_A, unsigned short op_B, unsigned char& FLAGS); // update ZF/NF/PF for compare

public:
    unsigned short execute(unsigned char OPCODE, unsigned short op_A, unsigned short op_B, unsigned char& FLAGS);
};

void ALU::ZF_update(unsigned short res, unsigned char& FLAGS)
{
    if (res == 0x0000) FLAGS |= RegisterBank::ZF; // result was zero
    else               FLAGS &= ~RegisterBank::ZF; // result was nonzero
}

void ALU::CMP_Flag_Update(unsigned short op_A, unsigned short op_B, unsigned char& FLAGS)
{
    FLAGS &= ~(RegisterBank::ZF | RegisterBank::NF | RegisterBank::PF); // clear all three first (mutually exclusive)

    if (op_A == op_B) FLAGS |= RegisterBank::ZF; // equal
    else if (op_A < op_B) FLAGS |= RegisterBank::NF; // left less than right
    else if (op_A > op_B) FLAGS |= RegisterBank::PF; // left greater than right
}

unsigned short ALU::execute(unsigned char OPCODE, unsigned short op_A, unsigned short op_B, unsigned char& FLAGS)
{
    unsigned short res = 0;
    switch (OPCODE)
    {
    case 0x01: res = op_A + op_B;                          ZF_update(res, FLAGS); break; // ADD
    case 0x02: res = op_A - op_B;                          ZF_update(res, FLAGS); break; // SUB
    case 0x03: res = op_A * op_B;                          ZF_update(res, FLAGS); break; // MUL
    case 0x04: res = (op_B == 0) ? 0x0000 : op_A / op_B;  ZF_update(res, FLAGS); break; // DIV - silent zero on div by zero
    case 0x0A: CMP_Flag_Update(op_A, op_B, FLAGS); return 0;                            // CMP - no writeback
    default: break;
    }
    return res;
}


// ================================================================
// MemoryModule
// ================================================================

// single flat 3840-byte RAM chip, byte addressed
// valid range: 0x0000 - 0x0EFF
// anything at 0x0F00 or above is a segfault
class MemoryModule
{
private:
    unsigned char storage[3840]; // 3840 bytes total as per spec

public:
    MemoryModule();
    unsigned char read_byte(unsigned short address);                              // runtime read - bounds checked
    void          write_byte(unsigned short address, unsigned char val);          // runtime write - bounds checked
    void          flash_byte(unsigned short address, unsigned char val);          // bootloader write - no bounds check
    void          loadRawBinary(unsigned short startAddress, unsigned short word); // writes a 16-bit word big-endian into two bytes
};

MemoryModule::MemoryModule()
{
    for (int i = 0; i < 3840; i++) storage[i] = 0x00; // power-on state: all cells 0
}

unsigned char MemoryModule::read_byte(unsigned short address)
{
    if (address >= 0x0F00) // out of bounds read
    {
        cout << "[HARDWARE FAULT] Segmentation Fault: Read out of bounds" << endl;
        return 0xFF; // data pathway floats to 0xFF
    }
    return storage[address];
}

void MemoryModule::write_byte(unsigned short address, unsigned char val)
{
    if (address >= 0x0F00) // out of bounds write
    {
        cout << "[HARDWARE FAULT] Segmentation Fault: Write out of bounds" << endl;
        return; // data discarded
    }
    storage[address] = val;
}

void MemoryModule::flash_byte(unsigned short address, unsigned char val)
{
    if (address < 3840) storage[address] = val; // bootloader bypass - no fault messages
}

void MemoryModule::loadRawBinary(unsigned short startAddress, unsigned short word)
{
    flash_byte(startAddress, (word >> 8) & 0xFF); // high byte first (big endian)
    flash_byte(startAddress + 1, word & 0xFF); // low byte second
}


// ================================================================
// PhosphorusDisplay
// ================================================================

// 32-column x 16-row CRT screen
// chars accumulate in buffer during execution, only shown at end via FLUSH_OUTPUT
class PhosphorusDisplay
{
private:
    char screen_buffer[16][32]; // 16 rows, 32 cols each
    int  current_row, current_col;

public:
    PhosphorusDisplay();
    void buffer_char(char c);  // add one char to screen buffer (handles newline and wrap)
    void FLUSH_OUTPUT();       // print entire buffer inside the bordered terminal frame
};

PhosphorusDisplay::PhosphorusDisplay()
{
    current_row = 0;
    current_col = 0;
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 32; j++)
            screen_buffer[i][j] = ' '; // blank the screen on startup
}

void PhosphorusDisplay::buffer_char(char c)
{
    if (c == '\n') // newline moves to next row
    {
        current_row++;
        current_col = 0;
    }
    else
    {
        if (current_row < 16) // rows beyond 16 are clipped (spec says so)
        {
            screen_buffer[current_row][current_col] = c;
            current_col++;
            if (current_col >= 32) // wrap to next row at col 32
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


// ================================================================
// GraphicsAdapter
// ================================================================

// sits between mainboard MMIO and the display
// receives raw byte payloads and translates them for the PhosphorusDisplay
class GraphicsAdapter
{
private:
    PhosphorusDisplay* target_display; // the screen plugged into this card

public:
    GraphicsAdapter();
    void plug_in_display(PhosphorusDisplay* d); // connect a screen to this card

    void CHAR_PAYLOAD(unsigned char val); // 0x0FF1 write - treat byte as ASCII char
    void INT_PAYLOAD(unsigned char val);  // 0x0FF2 write - treat byte as unsigned int, print digits
};

GraphicsAdapter::GraphicsAdapter() { target_display = nullptr; }

void GraphicsAdapter::plug_in_display(PhosphorusDisplay* d) { target_display = d; }

void GraphicsAdapter::CHAR_PAYLOAD(unsigned char val)
{
    if (target_display != nullptr) target_display->buffer_char((char)val); // forward as ascii char
}

void GraphicsAdapter::INT_PAYLOAD(unsigned char val)
{
    if (target_display == nullptr) return;

    if (val == 0) { target_display->buffer_char('0'); return; } // edge case: zero

    // extract digits in reverse then print forward
    char digits[4];
    int i = 0;
    unsigned char temp = val;
    while (temp > 0) { digits[i++] = (temp % 10) + '0'; temp /= 10; }
    while (i > 0)    target_display->buffer_char(digits[--i]);
}


// ================================================================
// Keyboard
// ================================================================

// external I/O panel - maintains a FIFO char buffer
// if buffer is empty when read, triggers a hardware interrupt and blocks until user types
class Keyboard
{
private:
    static const int MAX_CAPACITY = 512;

    char buffer[MAX_CAPACITY]; // circular FIFO buffer
    int  head, tail, count;

    void TRIGGER_INTERRUPT(); // blocks and reads a line from user into buffer

public:
    Keyboard();
    bool          PENDING_INPUT();  // true if buffer has at least one non-whitespace char
    unsigned char READ_CHAR();      // pop next non-whitespace char as ASCII byte
    unsigned char READ_INT();       // parse and pop next decimal integer token as 8-bit value
};

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

    // push all chars into the circular buffer
    for (int i = 0; input_line[i] != '\0'; i++)
    {
        if (count < MAX_CAPACITY)
        {
            buffer[tail] = input_line[i];
            tail = (tail + 1) % MAX_CAPACITY;
            count++;
        }
    }

    // append a space as delimiter so READ_INT can detect end of token
    if (count < MAX_CAPACITY)
    {
        buffer[tail] = ' ';
        tail = (tail + 1) % MAX_CAPACITY;
        count++;
    }
}

bool Keyboard::PENDING_INPUT()
{
    int h = head;
    for (int i = 0; i < count; i++)
    {
        if (buffer[h] != ' ' && buffer[h] != '\n' && buffer[h] != '\t') return true;
        h = (h + 1) % MAX_CAPACITY;
    }
    return false;
}

unsigned char Keyboard::READ_CHAR()
{
    while (!PENDING_INPUT()) TRIGGER_INTERRUPT(); // block until something is there

    // skip leading whitespace
    while (buffer[head] == ' ' || buffer[head] == '\n' || buffer[head] == '\t')
    {
        head = (head + 1) % MAX_CAPACITY;
        count--;
    }

    unsigned char val = (unsigned char)buffer[head];
    head = (head + 1) % MAX_CAPACITY;
    count--;
    return val;
}

unsigned char Keyboard::READ_INT()
{
    while (!PENDING_INPUT()) TRIGGER_INTERRUPT();

    unsigned int accumulator = 0;

    // skip non-digit chars
    while (count > 0 && (buffer[head] < '0' || buffer[head] > '9'))
    {
        head = (head + 1) % MAX_CAPACITY;
        count--;
    }

    // read digits until non-digit
    while (count > 0 && buffer[head] >= '0' && buffer[head] <= '9')
    {
        unsigned char digit = buffer[head] - '0';
        accumulator = (accumulator * 10) + digit;
        head = (head + 1) % MAX_CAPACITY;
        count--;
    }

    return (unsigned char)accumulator; // truncate to 8-bit result
}


// ================================================================
// Mainboard
// ================================================================

// central authority - every bus transaction in the system goes through here
// nothing communicates directly, everything routes through the mainboard
class Mainboard
{
private:
    unsigned short Address_bus;             // 16-bit address pathway
    unsigned char  Data_bus;               // 8-bit data pathway
    bool           READ_ENABLE, WRITE_ENABLE; // control pathway - only one asserted at a time

    bool system_power;   // goes false if PSU trips - halts all clock pulses
    bool memory_active;  // true during a RAM transaction (for PSU wattage polling)
    bool gpu_writing;    // true during a GPU write transaction (for PSU wattage polling)

    // component slots
    MemoryModule* ram_slot;
    Keyboard* key_slot;
    GraphicsAdapter* gpu_slot;

public:
    Mainboard();

    // slot connection methods
    void connect_ram(MemoryModule* m);
    void connect_keyboard(Keyboard* k);
    void connect_gpu(GraphicsAdapter* g);

    // bus transaction methods - these are the only way to move data in the system
    unsigned char bus_read(unsigned short address);
    void          bus_write(unsigned short address, unsigned char val);

    void clock_pulse(); // heartbeat - called once per bus transaction

    // state query methods for PSU wattage polling
    void killPower();
    bool is_powered();
    bool is_memory_active();
    bool is_gpu_active();
};

Mainboard::Mainboard()
{
    Address_bus = 0;
    Data_bus = 0;
    READ_ENABLE = false;
    WRITE_ENABLE = false;
    system_power = true;
    memory_active = false;
    gpu_writing = false;
    ram_slot = nullptr;
    key_slot = nullptr;
    gpu_slot = nullptr;
}

void Mainboard::connect_ram(MemoryModule* m) { ram_slot = m; }
void Mainboard::connect_keyboard(Keyboard* k) { key_slot = k; }
void Mainboard::connect_gpu(GraphicsAdapter* g) { gpu_slot = g; }
void Mainboard::clock_pulse() {}  // one pulse per transaction - timing hook

void Mainboard::killPower() { system_power = false; }
bool Mainboard::is_powered() { return system_power; }
bool Mainboard::is_memory_active() { return memory_active; }
bool Mainboard::is_gpu_active() { return gpu_writing; }

unsigned char Mainboard::bus_read(unsigned short address)
{
    Address_bus = address;
    READ_ENABLE = true;
    memory_active = (Address_bus <= 0x0EFF); // RAM range

    // MMIO intercept - address decoder routes before RAM ever sees the address
    if (key_slot && Address_bus == 0x0FF0) Data_bus = key_slot->READ_CHAR();            // keyboard char port
    else if (key_slot && Address_bus == 0x0FF3) Data_bus = key_slot->READ_INT();             // keyboard int port
    else if (ram_slot && Address_bus <= 0x0EFF) Data_bus = ram_slot->read_byte(Address_bus); // RAM
    else                                         Data_bus = 0x00;                            // unmapped - float to 0

    clock_pulse();
    READ_ENABLE = false;
    memory_active = false;
    return Data_bus;
}

void Mainboard::bus_write(unsigned short address, unsigned char val)
{
    Address_bus = address;
    Data_bus = val;
    WRITE_ENABLE = true;
    memory_active = (Address_bus <= 0x0EFF);
    gpu_writing = (Address_bus == 0x0FF1 || Address_bus == 0x0FF2);

    // MMIO intercept for display ports
    if (gpu_slot && Address_bus == 0x0FF1) gpu_slot->CHAR_PAYLOAD(Data_bus);             // display char port
    else if (gpu_slot && Address_bus == 0x0FF2) gpu_slot->INT_PAYLOAD(Data_bus);              // display int port
    else if (ram_slot && Address_bus <= 0x0EFF) ram_slot->write_byte(Address_bus, Data_bus);  // RAM
    // unmapped writes are silently discarded

    clock_pulse();
    WRITE_ENABLE = false;
    memory_active = false;
    gpu_writing = false;
}


// ================================================================
// PowerSupplyUnit
// ================================================================

// PSU polls wattage every clock cycle
// if draw exceeds rated capacity it calls killPower on the mainboard - halting everything
class PowerSupplyUnit
{
private:
    double     max_capacity;    // rated wattage set at manufacture time
    Mainboard* connected_board; // back-reference to the board it powers

public:
    PowerSupplyUnit(double watts);
    void plug_in(Mainboard* m); // connect to a mainboard
    void poll_wattage();        // check current draw and trip if over limit
};

PowerSupplyUnit::PowerSupplyUnit(double watts)
{
    max_capacity = watts;
    connected_board = nullptr;
}

void PowerSupplyUnit::plug_in(Mainboard* m) { connected_board = m; }

void PowerSupplyUnit::poll_wattage()
{
    if (connected_board == nullptr) return;

    double current_draw = 0.0;

    // RAM draws 0.5W active, 0.1W idle
    if (connected_board->is_memory_active()) current_draw += 0.5;
    else                                     current_draw += 0.1;

    // GPU draws 15W on a write, 2W idle
    if (connected_board->is_gpu_active()) current_draw += 15.0;
    else                                  current_draw += 2.0;

    if (current_draw > max_capacity) // over limit - trip the board
    {
        cout << "[CRITICAL WARNING] Power Draw exceeded PSU Capacity" << endl;
        connected_board->killPower();
    }
}


// ================================================================
// Interpreter
// ================================================================

struct CompilationResult
{
    vector<uint16_t> codeSegment;
    vector<uint16_t> dataSegment;
};

class Interpreter
{
public:
    Interpreter() {}

    CompilationResult compile(const string& sourceCode);
    void loadProgramAndFlash(const string& filename, MemoryModule& ram);

private:
    map<string, uint16_t> labelMap;
    vector<uint16_t>      parseLine(const string& line);
    uint8_t               parseRegister(const string& regStr);
    vector<string>        tokenize(const string& line);
};

vector<string> Interpreter::tokenize(const string& line)
{
    vector<string> tokens;
    stringstream   ss(line);
    string         token;

    while (ss >> token)
    {
        if (!token.empty() && token.back() == ',') token.pop_back();
        tokens.push_back(token);
    }
    return tokens;
}

uint8_t Interpreter::parseRegister(const string& regStr)
{
    string cleanReg = regStr;
    if (cleanReg.front() == '[') cleanReg.erase(0, 1);
    if (cleanReg.back() == ']') cleanReg.pop_back();

    if (cleanReg.size() >= 2 && cleanReg[0] == 'R')
    {
        int regNum = cleanReg[1] - '0';
        if (regNum >= 0 && regNum < 8) return static_cast<uint8_t>(regNum);
    }
    return 0;
}

vector<uint16_t> Interpreter::parseLine(const string& line)
{
    size_t commentPos = line.find("//");
    string cleanLine = (commentPos != string::npos) ? line.substr(0, commentPos) : line;

    vector<string> tokens = tokenize(cleanLine);
    if (tokens.empty()) return { 0x0000 }; // NOP

    string           mnemonic = tokens[0];
    vector<uint16_t> payload;

    if (mnemonic == "AARAM" || mnemonic == "NOP") {
        payload.push_back(0x0000);
    }
    else if (mnemonic == "JAMA" || mnemonic == "ADD") {
        uint8_t dest = parseRegister(tokens[1]);
        uint8_t src = parseRegister(tokens[2]);
        payload.push_back((0x01 << 8) | ((dest & 0x0F) << 4) | (src & 0x0F));
    }
    else if (mnemonic == "TAFREEK" || mnemonic == "SUB") {
        uint8_t dest = parseRegister(tokens[1]);
        uint8_t src = parseRegister(tokens[2]);
        payload.push_back((0x02 << 8) | ((dest & 0x0F) << 4) | (src & 0x0F));
    }
    else if (mnemonic == "ZARAB" || mnemonic == "MUL") {
        uint8_t dest = parseRegister(tokens[1]);
        uint8_t src = parseRegister(tokens[2]);
        payload.push_back((0x03 << 8) | ((dest & 0x0F) << 4) | (src & 0x0F));
    }
    else if (mnemonic == "TAQSEEM" || mnemonic == "DIV") {
        uint8_t dest = parseRegister(tokens[1]);
        uint8_t src = parseRegister(tokens[2]);
        payload.push_back((0x04 << 8) | ((dest & 0x0F) << 4) | (src & 0x0F));
    }
    else if (mnemonic == "MUWAZANA" || mnemonic == "CMP") {
        uint8_t dest = parseRegister(tokens[1]);
        uint8_t src = parseRegister(tokens[2]);
        payload.push_back((0x0A << 8) | ((dest & 0x0F) << 4) | (src & 0x0F));
    }
    else if (mnemonic == "CHHALANG" || mnemonic == "JMP") {
        uint8_t src = parseRegister(tokens[1]);
        payload.push_back((0x10 << 8) | (src & 0x0F));
    }
    else if (mnemonic == "AGAR_SIFAR" || mnemonic == "JZ") {
        uint8_t src = parseRegister(tokens[1]);
        payload.push_back((0x11 << 8) | (src & 0x0F));
    }
    else if (mnemonic == "AGAR_MAUJOOD" || mnemonic == "JNZ") {
        uint8_t src = parseRegister(tokens[1]);
        payload.push_back((0x12 << 8) | (src & 0x0F));
    }
    else if (mnemonic == "BHARO" || mnemonic == "DAALO" || mnemonic == "LDR_IMM") {
        uint8_t  dest = parseRegister(tokens[1]);
        uint16_t immFull = 0;

        bool isNumber = isdigit(tokens[2][0]) || (tokens[2].size() > 1 && tokens[2][1] == 'x');
        if (isNumber) {
            try { immFull = stoi(tokens[2], nullptr, 0); }
            catch (...) { cout << "[COMPILER ERROR] Invalid immediate format: " << tokens[2] << endl; }
        }
        else {
            if (labelMap.find(tokens[2]) != labelMap.end()) immFull = labelMap[tokens[2]];
            else cout << "[COMPILER ERROR] Unresolved Label: " << tokens[2] << endl;
        }

        // If it's a Label, it was tracked as 4 bytes in Pass 1, so we must force Format D
        if (!isNumber || immFull > 0x0F) {
            payload.push_back((0x1B << 8) | ((dest & 0x0F) << 4)); // Header word
            payload.push_back(immFull);                            // Payload word
        }
        else {
            // Fits in 4 bits (Format B)
            payload.push_back((0x1A << 8) | ((dest & 0x0F) << 4) | (immFull & 0x0F));
        }
    }
    else if (mnemonic == "PARHO" || mnemonic == "LDR") {
        uint8_t dest = parseRegister(tokens[1]);
        uint8_t src = parseRegister(tokens[2]);
        payload.push_back((0x20 << 8) | ((dest & 0x0F) << 4) | (src & 0x0F));
    }
    else if (mnemonic == "RAKHO" || mnemonic == "STR") {
        uint8_t srcVal = parseRegister(tokens[1]);
        uint8_t ptrReg = parseRegister(tokens[2]);
        payload.push_back((0x21 << 8) | ((srcVal & 0x0F) << 4) | (ptrReg & 0x0F));
    }
    else {
        cout << "[COMPILER ERROR] Unrecognized Mnemonic: " << mnemonic << endl;
    }

    return payload;
}

CompilationResult Interpreter::compile(const string& sourceCode)
{
    CompilationResult result;
    stringstream      ss(sourceCode);
    string            line;

    // storing pairs of {type, content}
    // type 0 = code, type 1 = data
    vector<pair<int, string>> processLines;

    labelMap.clear();
    uint16_t currentCodePC = 0;
    uint16_t currentDataPC = 0x0800;
    bool inDataSection = false;

    // Pass 1: PC Tracking and Label Mapping
    while (getline(ss, line))
    {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        if (line.empty() || line.find("//") == 0) continue;

        size_t commentPos = line.find("//");
        string cleanLine = (commentPos != string::npos) ? line.substr(0, commentPos) : line;
        cleanLine.erase(cleanLine.find_last_not_of(" \t\r\n") + 1);

        if (cleanLine == ".MAWAAD" || cleanLine == ".DATA" || cleanLine == ".mawaad" || cleanLine == ".data") {
            inDataSection = true;
            continue;
        }
        if (cleanLine == ".HIDAYAT" || cleanLine == ".CODE" || cleanLine == ".hidayat" || cleanLine == ".code") {
            inDataSection = false;
            continue;
        }

        if (inDataSection)
        {
            size_t colonPos = cleanLine.find(':');
            if (colonPos != string::npos)
            {
                string labelName = cleanLine.substr(0, colonPos);
                labelName.erase(labelName.find_last_not_of(" \t") + 1);
                labelMap[labelName] = currentDataPC;

                string valueStr = cleanLine.substr(colonPos + 1);
                valueStr.erase(0, valueStr.find_first_not_of(" \t"));

                processLines.push_back({ 1, valueStr });

                if (!valueStr.empty() && valueStr.front() == '"')
                {
                    // String literal
                    int chars = 0;
                    for (size_t i = 1; i < valueStr.size() && valueStr[i] != '"'; ++i)
                    {
                        if (valueStr[i] == '\\' && i + 1 < valueStr.size()) i++; // escape sequence
                        chars++;
                    }
                    int words = (chars + 1) / 2;
                    currentDataPC += words * 2;
                }
                else
                {
                    // Integer array or single int
                    int commas = count(valueStr.begin(), valueStr.end(), ',');
                    int words = commas + 1;
                    currentDataPC += words * 2;
                }
            }
        }
        else
        {
            if (cleanLine.back() == ':')
            {
                string labelName = cleanLine.substr(0, cleanLine.size() - 1);
                labelMap[labelName] = currentCodePC;
                continue; // Label takes 0 bytes
            }

            processLines.push_back({ 0, cleanLine });

            vector<string> tokens = tokenize(cleanLine);
            if (tokens.empty()) continue;

            // Calculate instruction byte size
            string mnemonic = tokens[0];
            if (mnemonic == "BHARO" || mnemonic == "DAALO" || mnemonic == "LDR_IMM")
            {
                if (tokens.size() > 2)
                {
                    bool isNumber = isdigit(tokens[2][0]) || (tokens[2].size() > 1 && tokens[2][1] == 'x');
                    if (isNumber)
                    {
                        try {
                            uint16_t immFull = stoi(tokens[2], nullptr, 0);
                            if (immFull <= 0x0F) currentCodePC += 2;
                            else                 currentCodePC += 4; // Format D
                        }
                        catch (...) { currentCodePC += 4; } // Fallback
                    }
                    else currentCodePC += 4; // Labels unconditionally 2 words
                }
                else currentCodePC += 2;
            }
            else currentCodePC += 2; // Fixed width defaults
        }
    }

    // Pass 2: Instruction Generation
    for (const auto& item : processLines)
    {
        if (item.first == 1) // DATA
        {
            string valueStr = item.second;
            if (!valueStr.empty() && valueStr.front() == '"')
            {
                vector<uint8_t> bytes;
                for (size_t i = 1; i < valueStr.size() && valueStr[i] != '"'; ++i)
                {
                    if (valueStr[i] == '\\' && i + 1 < valueStr.size())
                    {
                        i++;
                        if (valueStr[i] == '0') bytes.push_back(0);
                        else if (valueStr[i] == 'n') bytes.push_back('\n');
                        else                         bytes.push_back(valueStr[i]);
                    }
                    else bytes.push_back(valueStr[i]);
                }
                if (bytes.size() % 2 != 0) bytes.push_back(0); // Word alignment
                for (size_t i = 0; i < bytes.size(); i += 2)
                    result.dataSegment.push_back((bytes[i] << 8) | bytes[i + 1]);
            }
            else
            {
                stringstream vss(valueStr);
                string       token;
                while (getline(vss, token, ','))
                {
                    token.erase(0, token.find_first_not_of(" \t"));
                    token.erase(token.find_last_not_of(" \t") + 1);
                    try { result.dataSegment.push_back(stoi(token, nullptr, 0)); }
                    catch (...) { result.dataSegment.push_back(0); }
                }
            }
        }
        else // CODE
        {
            vector<uint16_t> words = parseLine(item.second);
            for (uint16_t word : words) result.codeSegment.push_back(word);
        }
    }

    return result;
}

void Interpreter::loadProgramAndFlash(const string& filename, MemoryModule& ram)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "[BIOS FAULT] Could not locate OS payload: " << filename << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string targetOS = buffer.str();

    CompilationResult compiledOS = compile(targetOS);
    cout << "[BIOS] Compiled OS script into " << compiledOS.codeSegment.size() << " native 16-bit instruction words." << endl;

    cout << "[BOOTLOADER] Flashing Code Execution block to RAM Address 0x0000..." << endl;
    uint16_t codeAddress = 0x0000;
    for (uint16_t word : compiledOS.codeSegment) { ram.loadRawBinary(codeAddress, word); codeAddress += 2; }

    cout << "[BOOTLOADER] Flashing Data Variables (.MAWAAD) to RAM Address 0x0800..." << endl;
    uint16_t dataAddress = 0x0800;
    for (uint16_t word : compiledOS.dataSegment) { ram.loadRawBinary(dataAddress, word); dataAddress += 2; }

    cout << "[BOOTLOADER] Flash Complete." << endl;
    cout << "------------------------------------------------------------\n" << endl;
}


// ================================================================
// SignalVector + Processor
// ================================================================

// one row in the decode matrix - burned in at manufacture time
// tells the execution datapath exactly what to do for each opcode
struct SignalVector
{
    bool          IS_VALID;           // false = invalid opcode, emit hardware fault
    bool          USES_AEC;           // invoke the ALU
    unsigned char AEC_OPERATION;      // which ALU operation to run
    bool          IS_MEMORY_READ;     // load a byte from memory into dest
    bool          IS_MEMORY_WRITE;    // store a byte from src into memory at dest address
    bool          IS_BRANCH;          // redirect the PC
    bool          BRANCH_ON_ZERO;     // conditional - only branch if ZF is set
    bool          BRANCH_ON_NOT_ZERO; // conditional - only branch if ZF is clear
    bool          WRITES_TO_REGISTER; // write execution result back to dest register
    bool          IS_4BIT_IMMEDIATE;  // result is the 4-bit imm packed in the low nibble
    bool          IS_16BIT_IMMEDIATE; // result is a full 16-bit value from next word (Format D)
};

// single chip processor - contains storage bank, ALU, cache, and decode matrix
class Processor
{
private:
    RegisterBank storage_bank;  // all 11 registers
    ALU          aec;           // arithmetic execution core
    Mainboard* mainboard_ref; // the board this chip is seated in

    unsigned char  cache_data[16];  // 16-byte block cache
    unsigned short cache_base_addr; // base address of the currently cached block
    bool           cache_valid;     // whether cache holds valid data right now

    SignalVector decode_matrix[256]; // 256-entry lookup table indexed by opcode byte
    double       current_temp;       // chip temperature in Celsius

    void          init_decode_matrix();                     // burn in the signal vectors at startup
    unsigned char fetch_with_cache(unsigned short address); // fetch one byte, using cache if possible

public:
    Processor();
    void plug_in(Mainboard* m); // seat this processor into a mainboard socket
    bool step();                // execute one full fetch-decode-execute cycle, returns false when halted

    RegisterBank& get_storage() { return storage_bank; }
    double        get_temperature() { return current_temp; }
};

Processor::Processor()
{
    mainboard_ref = nullptr;
    current_temp = 25.0; // base temp at startup per spec
    cache_valid = false;
    cache_base_addr = 0;
    init_decode_matrix();
}

void Processor::plug_in(Mainboard* m) { mainboard_ref = m; }

unsigned char Processor::fetch_with_cache(unsigned short address)
{
    // cache hit - address falls inside the currently loaded 16-byte block
    if (cache_valid && address >= cache_base_addr && address < cache_base_addr + 16)
        return cache_data[address - cache_base_addr];

    // cache miss - load a fresh 16-byte block aligned to the boundary
    cache_base_addr = address & 0xFFF0;
    for (int i = 0; i < 16; i++)
        cache_data[i] = mainboard_ref->bus_read(cache_base_addr + i);

    cache_valid = true;
    return cache_data[address - cache_base_addr];
}

void Processor::init_decode_matrix()
{
    // mark every opcode invalid first, then fill in the known ones
    for (int i = 0; i < 256; i++) decode_matrix[i].IS_VALID = false;

    //                                     VALID  AEC    OP     MR     MW     BR     BZ     BNZ    WR     4BI    16BI
    decode_matrix[0x00] = { true,  false, 0x00,  false, false, false, false, false, false, false, false }; // NOP / AARAM
    decode_matrix[0x01] = { true,  true,  0x01,  false, false, false, false, false, true,  false, false }; // ADD / JAMA
    decode_matrix[0x02] = { true,  true,  0x02,  false, false, false, false, false, true,  false, false }; // SUB / TAFREEK
    decode_matrix[0x03] = { true,  true,  0x03,  false, false, false, false, false, true,  false, false }; // MUL / ZARAB
    decode_matrix[0x04] = { true,  true,  0x04,  false, false, false, false, false, true,  false, false }; // DIV / TAQSEEM
    decode_matrix[0x0A] = { true,  true,  0x0A,  false, false, false, false, false, false, false, false }; // CMP / MUWAZANA - no writeback
    decode_matrix[0x10] = { true,  false, 0x00,  false, false, true,  false, false, false, false, false }; // JMP / CHHALANG - unconditional
    decode_matrix[0x11] = { true,  false, 0x00,  false, false, true,  true,  false, false, false, false }; // JZ  / AGAR_SIFAR
    decode_matrix[0x12] = { true,  false, 0x00,  false, false, true,  false, true,  false, false, false }; // JNZ / AGAR_MAUJOOD
    decode_matrix[0x1A] = { true,  false, 0x00,  false, false, false, false, false, true,  true,  false }; // LDR_IMM 4-bit / BHARO Format B
    decode_matrix[0x1B] = { true,  false, 0x00,  false, false, false, false, false, true,  false, true }; // LDR_IMM 16-bit / BHARO Format D
    decode_matrix[0x20] = { true,  false, 0x00,  true,  false, false, false, false, true,  false, false }; // LDR / PARHO
    decode_matrix[0x21] = { true,  false, 0x00,  false, true,  false, false, false, false, false, false }; // STR / RAKHO
}

bool Processor::step()
{
    unsigned char flags = storage_bank.readFLAGS();

    // halt flag check - if HF is set the processor permanently stops
    if (flags & RegisterBank::HF) return false;

    // no mainboard seated or board lost power
    if (mainboard_ref && !mainboard_ref->is_powered()) return false;

    // FETCH - two bus reads to get the 16-bit instruction word
    unsigned short pc = storage_bank.readPC();
    unsigned char  high = fetch_with_cache(pc);
    unsigned char  low = fetch_with_cache(pc + 1);
    unsigned short ir = (high << 8) | low;

    if (ir == 0x0000) return false; // NOP / AARAM signals end of program

    storage_bank.writeIR(ir);
    storage_bank.writePC(pc + 2); // increment PC before decode so branches work correctly

    // DECODE - look up the signal vector for this opcode byte
    SignalVector signals = decode_matrix[high];

    if (!signals.IS_VALID)
    {
        cout << "[HARDWARE FAULT] Invalid Instruction" << endl;
        return true; // cycle aborted, execution continues at next PC
    }

    // extra fetch for Format D (16-bit immediate) - two more bus reads
    unsigned short immediate_payload = 0;
    if (signals.IS_16BIT_IMMEDIATE)
    {
        unsigned short npc = storage_bank.readPC();
        immediate_payload = (fetch_with_cache(npc) << 8) | fetch_with_cache(npc + 1);
        storage_bank.writePC(npc + 2); // skip over the payload word
    }

    // extract dest and src nibbles from instruction word
    unsigned char  dest = (ir >> 4) & 0x0F;
    unsigned char  src = ir & 0x0F;
    unsigned short valA = storage_bank.readR(dest);
    unsigned short valB = storage_bank.readR(src);
    unsigned short result = 0;

    // EXECUTE - datapath follows signals mechanically
    if (signals.USES_AEC)
    {
        result = aec.execute(signals.AEC_OPERATION, valA, valB, flags);
        storage_bank.writeFLAGS(flags); // ALU updated flags through its direct connection
    }
    else if (signals.IS_MEMORY_READ)
        result = mainboard_ref->bus_read(valB); // load byte from address in src register

    else if (signals.IS_MEMORY_WRITE)
    {
        mainboard_ref->bus_write(valB, (unsigned char)valA); // store src value at address in dest
        if (cache_valid && valB >= cache_base_addr && valB < cache_base_addr + 16)
            cache_valid = false; // invalidate cache if we wrote into the cached block
    }
    else if (signals.IS_BRANCH)
    {
        bool take = true;
        if (signals.BRANCH_ON_ZERO && !(flags & RegisterBank::ZF)) take = false; // JZ  - only if ZF set
        if (signals.BRANCH_ON_NOT_ZERO && (flags & RegisterBank::ZF)) take = false; // JNZ - only if ZF clear
        if (take) storage_bank.writePC(valB); // load target address from src register into PC
    }
    else if (signals.IS_4BIT_IMMEDIATE)
        result = src; // the 4-bit imm is already sitting in the low nibble of the instruction

    else if (signals.IS_16BIT_IMMEDIATE)
        result = immediate_payload; // use the full 16-bit word we fetched during decode

    // WRITEBACK
    if (signals.WRITES_TO_REGISTER)
        storage_bank.writeR(dest, result);

    // THERMODYNAMICS - temp rises 0.05C every cycle
    current_temp += 0.05;
    if (current_temp >= 90.0) // thermal shutdown threshold
    {
        cout << "[CRITICAL ERROR] Thermal threshold exceeded" << endl;
        flags |= RegisterBank::HF;
        storage_bank.writeFLAGS(flags); // set HF - permanent halt
        return false;
    }

    return true;
}