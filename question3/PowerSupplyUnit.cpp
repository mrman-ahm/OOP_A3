#include "PowerSupplyUnit.h"
#include "Mainboard.h"
#include <iostream>

using namespace std;

PowerSupplyUnit::PowerSupplyUnit(double watts)
{
    max_capacity = watts;
    connected_board = nullptr;
}

void PowerSupplyUnit::plug_in(Mainboard* m)
{
    connected_board = m;
}

void PowerSupplyUnit::poll_wattage()
{
    if (connected_board == nullptr) return;

    double current_draw = 0.0;

    if (connected_board->is_memory_active())
        current_draw += 0.5;
    else
        current_draw += 0.1;

    if (connected_board->is_gpu_active())
        current_draw += 15.0;
    else
        current_draw += 2.0;

    if (current_draw > max_capacity)
    {
        cout << "[CRITICAL WARNING] Power Draw exceeded PSU Capacity" << endl;
        connected_board->killPower();
    }
}