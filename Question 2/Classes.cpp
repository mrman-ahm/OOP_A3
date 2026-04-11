#include "Classes.h"





//Asset Functions
void Asset::takeDmg(int damage){ int dr = damage - armor; if(dr<0) dr=0; hp -= dr; if(hp<0) hp=0; }
void Asset::takedefaultDmg(int damage){ hp -= damage; if(hp<0) hp=0; } // $$ This just might be faulty





// FROSTY PEAK FUNCITONS
void FrostPeaksKingdom::applyColdWeather() {
    for (int i = 0; i < footSoldierCount; i++) if (footSoldiers[i]) footSoldiers[i]->takedefaultDmg(coldAttritionModifier);
    for (int i = 0; i < knightCount; i++) if (knights[i]) knights[i]->takedefaultDmg(coldAttritionModifier);
    for (int i = 0; i < cavalryCount; i++) if (cavalry[i]) cavalry[i]->takedefaultDmg(coldAttritionModifier);
    for (int i = 0; i < warriorLordCount; i++) if (warriorLords[i]) warriorLords[i]->Knight::takedefaultDmg(coldAttritionModifier);
    for (int i = 0; i < ramCount; i++) if (rams[i]) rams[i]->takedefaultDmg(coldAttritionModifier);
    for (int i = 0; i < catapultCount; i++) if (catapults[i]) catapults[i]->takedefaultDmg(coldAttritionModifier);
    for (int i = 0; i < warShipCount; i++) if (warShips[i]) warShips[i]->Transport::takedefaultDmg(coldAttritionModifier);
}

Kingdom::~Kingdom() {
    // Ownership Violation Fix: Kingdom only deletes its LOCAL arrays of pointers.
    // It must NOT delete the entities themselves (Engine owns them).
    delete[] footSoldiers;
    delete[] knights;
    delete[] cavalry;
    delete[] rams;
    delete[] catapults;
    delete[] warShips;
    delete[] courtiers;
    delete[] warriorLords;
    delete[] sages;
    delete[] heirIds;
}

void terminateLord(Noble* n) {
    if (!n) return;
    if (n->typeCode == 1) delete (LandedLord*)n;
    else if (n->typeCode == 2) delete (LandlessLord*)n;
    else if (n->typeCode == 3) delete (WarriorLord*)n;
    else delete n;
}