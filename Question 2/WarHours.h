#ifndef WAR_HOURS_H
#define WAR_HOURS_H

#include "Classes.h"
#include <iostream>

struct WarHours {
    Kingdom* attacker;
    Kingdom* defender;
    

    int presentHour; 
    WarHours(Kingdom* atk, Kingdom* def) : attacker(atk), defender(def), presentHour(0) {}


    int figureOutWhoWins();

    void doPostWarStuff(Kingdom* attacker, Kingdom* defender, int result, int** relations, int countofrealms);



private:


    void killTheUnits(Kingdom* target, int incomingDamage);


    int generateLineDamage(Kingdom* k, float strategyMod);

    int getFootSoldiersTotalHP(Kingdom* k);

    int generateHealing(Kingdom* k, int incomingInfantryDamage);

    int calculateTotalPower(Kingdom* k);




    int generateSiegeDamage(Kingdom* k, float strategyMod);

    int generatePhaseDamage(Kingdom* k, float strategyMod, int phaseType);

    float getStrategyModifier(Kingdom* k);


    void applyNavalAttrition(Kingdom* target);
    
    int generateFlankDamage(Kingdom* k, float strategyMod);
    
    int generateEliteDamage(Kingdom* k, float strategyMod);



};




#endif
