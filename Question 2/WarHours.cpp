#include "WarHours.h"

int WarHours::calculateTotalPower(Kingdom* k) {
    if (!k) return 0;
    
    int power = (k->countoffootsoldiers * 1) + (k->countofcavalry * 5) + (k->countofknights * 8) + (k->countofships * 12) + (k->countoframs * 25) + (k->countofwarriorlords * 10);
    return power;
}

float WarHours::getStrategyModifier(Kingdom* k) {
    float baseModifier = 1.0f;
    if (!k) return baseModifier;
    
    if (k->getCurrentLord()) baseModifier += (k->getCurrentLord()->strategyStat / 100.0f);
    
    return baseModifier;
}

int WarHours::generatePhaseDamage(Kingdom* k, float strategyMod, int phaseType) {
    if (phaseType == 1) return generateSiegeDamage(k, strategyMod);
    if (phaseType == 2) return generateFlankDamage(k, strategyMod);
    if (phaseType == 3) return generateEliteDamage(k, strategyMod);
    if (phaseType == 4) return generateLineDamage(k, strategyMod);
    return 0;
}

int WarHours::generateSiegeDamage(Kingdom* k, float strategyMod) {
    if (!k) return 0;
    int dmg = 0;
    
    for (int i = 0; i < k->countofships; i++) { if (k->warShips[i]) dmg += k->warShips[i]->SiegeEngine::siegeDamage; }
    for (int i = 0; i < k->countofcatapults; i++) { if (k->catapults[i]) dmg += k->catapults[i]->siegeDamage; }
    for (int i = 0; i < k->countoframs; i++) { if (k->rams[i]) dmg += k->rams[i]->siegeDamage; }
    
    int totaldamage = (int)(dmg * strategyMod);
    return totaldamage;
}

int WarHours::generateFlankDamage(Kingdom* k, float strategyMod) {
    if (!k) return 0;
    int dmg = 0;
    
    for (int i = 0; i < k->countofcavalry; i++) { if (k->cavalry[i]) dmg += (int)(k->cavalry[i]->attackPower * k->cavalry[i]->chargeMultiplier); }
    
    int totaldamage = (int)(dmg * strategyMod);
    return totaldamage;
}

int WarHours::generateEliteDamage(Kingdom* k, float strategyMod) {
    if (!k) return 0;
    int dmg = 0;
    
    for (int i = 0; i < k->countofknights; i++) { if (k->knights[i]) dmg += k->knights[i]->attackPower; }
    for (int i = 0; i < k->countofwarriorlords; i++) { if (k->warriorLords[i]) dmg += k->warriorLords[i]->Knight::attackPower; }
    
    int totaldamage = (int)(dmg * strategyMod);     
    return totaldamage;
}

int WarHours::generateLineDamage(Kingdom* k, float strategyMod) {
    if (!k) return 0;
    
    int dmg = k->countoffootsoldiers * 10;
    
    int totaldamage = (int)(dmg * strategyMod);
    return totaldamage;
}

int WarHours::generateHealing(Kingdom* k, int incomingInfantryDamage) {
    if (!k || incomingInfantryDamage <= 0) return 0; 
    
    int totalSageHealing = 0;
    for(int i = 0; i < k->countofsages; ++i) { if(k->sages[i]) totalSageHealing += k->sages[i]->healingPower; }
    
    int maxAllowed = (int)(incomingInfantryDamage * 0.01f);
    int restoredFoot = (totalSageHealing < maxAllowed) ? totalSageHealing : maxAllowed;
    
    k->countoffootsoldiers += restoredFoot;
    
    return restoredFoot;
}

void WarHours::killTheUnits(Kingdom* target, int incomingDamage) {
    if (!target || incomingDamage <= 0) return;
    
    int casualtyPoints = (int)(incomingDamage * 0.05f); 
    if (casualtyPoints <= 0) return;

    while (target->countofcavalry > 0 && casualtyPoints > 0) {
        Cavalry* unit = target->cavalry[target->countofcavalry - 1];
        if (unit) {
            if (casualtyPoints >= unit->hp) {
                casualtyPoints -= unit->hp;
                target->cavalry[target->countofcavalry - 1] = nullptr;
                target->countofcavalry--;
            } else {
                unit->hp -= casualtyPoints;
                casualtyPoints = 0;
            }
        } else { target->countofcavalry--; }
    }

    while (target->countofknights > 0 && casualtyPoints > 0) {
        Knight* unit = target->knights[target->countofknights - 1];
        if (unit) {
            int effectiveDmg = casualtyPoints;
            if (unit->isShieldActive()) {
                effectiveDmg -= 2; if (effectiveDmg < 0) effectiveDmg = 0;
            }

            if (effectiveDmg >= unit->hp) {
                casualtyPoints -= unit->hp; 
                target->knights[target->countofknights - 1] = nullptr;
                target->countofknights--;
            } else {
                unit->hp -= effectiveDmg;
                casualtyPoints = 0; 
            }
        } else { target->countofknights--; }
    }
    while (target->countofwarriorlords > 0 && casualtyPoints > 0) {
        WarriorLord* unit = target->warriorLords[target->countofwarriorlords - 1];
        if (unit) {
            int effectiveDmg = casualtyPoints;
            Knight* kUnit = (Knight*)unit;
            if (kUnit->isShieldActive()) {
                effectiveDmg -= 2; if (effectiveDmg < 0) effectiveDmg = 0;
            }

            if (effectiveDmg >= kUnit->hp) {
                casualtyPoints -= kUnit->hp;
                target->warriorLords[target->countofwarriorlords - 1] = nullptr;
                target->countofwarriorlords--;
            } else {
                kUnit->hp -= effectiveDmg;
                casualtyPoints = 0;
            }
        } else { target->countofwarriorlords--; }
    }

    while (target->countoffootsoldiers > 0 && casualtyPoints > 0) {
        FootSoldier* unit = target->footSoldiers[target->countoffootsoldiers - 1];
        if (unit) {
            if (casualtyPoints >= unit->hp) {
                casualtyPoints -= unit->hp;
                target->footSoldiers[target->countoffootsoldiers - 1] = nullptr;
                target->countoffootsoldiers--;
            } else {
                unit->hp -= casualtyPoints;
                casualtyPoints = 0;
            }
        } else { target->countoffootsoldiers--; }
    }
}

void WarHours::applyNavalAttrition(Kingdom* target) {
    if (!target) return;
    
    if (target->countofships > 0) {
        int loss = (int)(target->countofships * 0.1f); if (loss < 1) loss = 1;
        target->countofships -= loss; if (target->countofships < 0) target->countofships = 0;
    }
}

int WarHours::getFootSoldiersTotalHP(Kingdom* k) {
    if (!k) return 0;
    int totalHP = 0;
    for (int i = 0; i < k->countoffootsoldiers; i++) { if (k->footSoldiers[i]) totalHP += k->footSoldiers[i]->hp; }
    return totalHP;
}
    
int WarHours::figureOutWhoWins() {
    if (calculateTotalPower(attacker) == 0) return 2;
    
    while (presentHour < 24) {
        std::cout << "\n  HOUR " << (presentHour + 1) << "\n";
        float mAtk = getStrategyModifier(attacker);
        float mDef = getStrategyModifier(defender);
        
        int siegeAtk = generatePhaseDamage(attacker, mAtk, 1);
        int siegeDef = generatePhaseDamage(defender, mDef, 1);
        std::cout << "  siege...: Atk: " << siegeAtk << " | Def: " << siegeDef << "\n";
        killTheUnits(defender, siegeAtk);
        killTheUnits(attacker, siegeDef);

        int flankAtk = generatePhaseDamage(attacker, mAtk, 2);
        int flankDef = generatePhaseDamage(defender, mDef, 2);
        std::cout << "  flank...: Atk: " << flankAtk << " | Def: " << flankDef << "\n";
        killTheUnits(defender, flankAtk);
        killTheUnits(attacker, flankDef);

        int eliteAtk = generatePhaseDamage(attacker, mAtk, 3);
        int eliteDef = generatePhaseDamage(defender, mDef, 3);
        std::cout << "  elite...: Atk: " << eliteAtk << " | Def: " << eliteDef << "\n";
        killTheUnits(defender, eliteAtk);
        killTheUnits(attacker, eliteDef);

        int lineAtk = generatePhaseDamage(attacker, mAtk, 4);
        int lineDef = generatePhaseDamage(defender, mDef, 4);
        std::cout << "  line...:  Atk: " << lineAtk << " | Def: " << lineDef << "\n";
        killTheUnits(defender, lineAtk);
        killTheUnits(attacker, lineDef);
        
        applyNavalAttrition(attacker);
        applyNavalAttrition(defender);
        
        int healDef = generateHealing(defender, lineAtk);
        int healAtk = generateHealing(attacker, lineDef);

        std::cout << "  heal...:  Atk: " << healAtk << " | Def: " << healDef << "\n";
        
        int presentAtk = calculateTotalPower(attacker);
        int presentDef = calculateTotalPower(defender);
        
        if (presentAtk < (int)(presentDef * 0.5f)) return 1;  
        if (presentDef < (int)(presentAtk * 0.5f)) return -1; 
        presentHour++;
    }
    
    return 0; 
}

void WarHours::doPostWarStuff(Kingdom* attacker, Kingdom* defender, int result, int** relations, int countofrealms) {
    if (!attacker || !defender) return;

    int atkIdx = attacker->realmID;
    int defIdx = defender->realmID;

    if (result == -1) { 
        int loot = (int)(defender->wealth * 0.75f);
        attacker->wealth += loot;
        defender->wealth -= loot;
        if (defender->wealth < 0) defender->wealth = 0;
    }

    if (result == -1) { 
        relations[atkIdx][defIdx] = 0;
        relations[defIdx][atkIdx] = -50;
    } else { 
        relations[atkIdx][defIdx] = 0;
        relations[defIdx][atkIdx] = 0;
    }

    for (int i = 0; i < countofrealms; i++) {
        if (i != atkIdx) {
            relations[i][atkIdx] -= 2;
            if (relations[i][atkIdx] < -100) relations[i][atkIdx] = -100;
        }
    }
}
