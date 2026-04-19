#include <iostream>
#include "Header.h"
using namespace std;

int main()
{
    // create all hardware components
    Mainboard         motherboard;
    MemoryModule      ram;
    Keyboard          kbd;
    PhosphorusDisplay crt;
    GraphicsAdapter   gpu;
    PowerSupplyUnit   psu(40.0); // 40W rated capacity
    Processor         cpu;
    Interpreter       bootloader;

    // plug everything into the mainboard
    motherboard.connect_ram(&ram);
    motherboard.connect_keyboard(&kbd);
    motherboard.connect_gpu(&gpu);
    gpu.plug_in_display(&crt);
    psu.plug_in(&motherboard);
    cpu.plug_in(&motherboard);

    // boot menu - pick which program to flash and run
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

    // run until step returns false (halt, thermal shutdown, or power cut)
    while (cpu.step())
    {
        psu.poll_wattage();
        if (!motherboard.is_powered()) break;
    }

    cout << "\n--- Execution Finished ---" << endl;

    crt.FLUSH_OUTPUT(); // dump the phosphor display to terminal

    return 0;
}