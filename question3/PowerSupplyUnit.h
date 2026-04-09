#pragma once

class Mainboard;

class PowerSupplyUnit
{
private:
    double max_capacity;
    Mainboard* connected_board;
public:
    PowerSupplyUnit(double watts);
    void plug_in(Mainboard* m);
    void poll_wattage();
};