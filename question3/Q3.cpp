#include <iostream>
#include "Mainboard.h"
#include "Processor.h"
#include "MemoryModule.h"
#include "Keyboard.h"
#include "PhosphorusDisplay.h"
#include "GraphicsAdapter.h"
#include "PowerSupplyUnit.h"
#include "Interpreter.h"

using namespace std;

int main()
{
    Mainboard motherboard;
    MemoryModule ram;
    Keyboard kbd;
    PhosphorusDisplay crt;
    GraphicsAdapter gpu;
    PowerSupplyUnit psu(40.0);
    Processor cpu;
    Interpreter bootloader;

    motherboard.connect_ram(&ram);
    motherboard.connect_keyboard(&kbd);
    motherboard.connect_gpu(&gpu);
    gpu.plug_in_display(&crt);
    psu.plug_in(&motherboard);
    cpu.plug_in(&motherboard);

    cout << "ONYX-16 BOOT MENU" << endl;
    cout << "-----------------" << endl;
    cout << "[1] Calculator (calculator.txt)" << endl;
    cout << "[2] Hello World (hello.txt)" << endl;
    cout << "[3] Firewall (auth.txt)" << endl;
    cout << "Selection: ";

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) bootloader.loadProgramAndFlash("calculator.txt", ram);
    else if (choice == 2) bootloader.loadProgramAndFlash("hello.txt", ram);
    else if (choice == 3) bootloader.loadProgramAndFlash("auth.txt", ram);
    else return 0;

    cout << "Starting Execution..." << endl;

    while (cpu.step())
    {
        psu.poll_wattage();
        if (!motherboard.is_powered()) break;
    }

    cout << "\n--- Execution Finished ---" << endl;

    crt.FLUSH_OUTPUT();

    return 0;
}