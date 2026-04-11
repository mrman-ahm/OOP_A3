#include "WarHours.h"

int WarHours::calculateTotalPower(Kingdom* k) {
    if (!k) return 0;
    
    // Formula from assignments requirements: 
    // Power = (FootSoldiers * 1) + (Cavalry * 5) + (Knights * 8) + (Ships * 12) + (Rams * 25) + (WarriorLords * 10)
    int power = (k->footSoldierCount * 1) + (k->cavalryCount * 5) + (k->knightCount * 8) + (k->warShipCount * 12) + (k->ramCount * 25) + (k->warriorLordCount * 10);
    return power;
}

float WarHours::getStrategyModifier(Kingdom* k) {
    // Modifier(M) = 1.0 + (CommandingLord.StrategyStat / 100.0)
    float baseModifier = 1.0f;
    if (!k) return baseModifier;
    
    // Efficient Pointer Architecture: Accessing the ruler directly
    if (k->currentLord) baseModifier += (k->currentLord->strategyStat / 100.0f);
    
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
    
    // SUM(RamDmg + CatapultDmg + ShipSiegeDmg) * M
    for (int i = 0; i < k->warShipCount; i++) { if (k->warShips[i]) dmg += k->warShips[i]->SiegeEngine::siegeDamage; }
    for (int i = 0; i < k->catapultCount; i++) { if (k->catapults[i]) dmg += k->catapults[i]->siegeDamage; }
    for (int i = 0; i < k->ramCount; i++) { if (k->rams[i]) dmg += k->rams[i]->siegeDamage; }
    
    int totaldamage = (int)(dmg * strategyMod);
    return totaldamage;
}

int WarHours::generateFlankDamage(Kingdom* k, float strategyMod) {
    if (!k) return 0;
    int dmg = 0;
    
    // SUM(CavAttack * CavChargeMultiplier) * M
    for (int i = 0; i < k->cavalryCount; i++) { if (k->cavalry[i]) dmg += (int)(k->cavalry[i]->attackPower * k->cavalry[i]->chargeMultiplier); }
    
    int totaldamage = (int)(dmg * strategyMod);
    return totaldamage;
}

int WarHours::generateEliteDamage(Kingdom* k, float strategyMod) {
    if (!k) return 0;
    int dmg = 0;
    
    // (SUM(KnightAttack) + SUM(WarriorLordAttack)) * M
    for (int i = 0; i < k->knightCount; i++) { if (k->knights[i]) dmg += k->knights[i]->attackPower; }
    for (int i = 0; i < k->warriorLordCount; i++) { if (k->warriorLords[i]) dmg += k->warriorLords[i]->Knight::attackPower; }
    
    int totaldamage = (int)(dmg * strategyMod); // casting into innt bcause assinment is mostly integrating ints like int hp etc    
    return totaldamage;
}

int WarHours::generateLineDamage(Kingdom* k, float strategyMod) {
    if (!k) return 0;
    
    // (FootCount  *  10)  *  M
    // (Note: The assignment explicitly hardcodes 10 as generic combat meat grinder math, ignoring Foot.Attack natively)
    int dmg = k->footSoldierCount * 10;
    
    int totaldamage = (int)(dmg * strategyMod);
    return totaldamage;
}

int WarHours::generateHealing(Kingdom* k, int incomingInfantryDamage) {
    if (!k || incomingInfantryDamage <= 0) return 0; // nobles cavalry and warrior lords cant be healed as they are nobles. so only heal the damage on footsoldiers
    
    int totalSageHealing = 0;
    for(int i = 0; i < k->sageCount; ++i) { if(k->sages[i]) totalSageHealing += k->sages[i]->healingPower; }
    
    // Triage Rule: MIN(SUM(SageHealing), floor(IncomingInfantryDamage * 0.01))
    int maxAllowed = (int)(incomingInfantryDamage * 0.01f);
    int restoredFoot = (totalSageHealing < maxAllowed) ? totalSageHealing : maxAllowed;
    
    // Rule constraint: "re-activating incapacitated units. No new FootSoldier objects are allocated; the count is directly incremented."
    // Because healing is bound tightly by <1% of the incoming damage mathematically, the pointer addresses simply wake back up dynamically
    // without ever exceeding the memory allocation maximum.
    k->footSoldierCount += restoredFoot;
    
    return restoredFoot;
}

void WarHours::applyNavalAttrition(Kingdom* target) {
    if (!target) return;
    
    if (target->warShipCount > 0) {
        int loss = (int)(target->warShipCount * 0.1f); if (loss < 1) loss = 1;
        target->warShipCount -= loss; if (target->warShipCount < 0) target->warShipCount = 0;
    }
}

int WarHours::getFootSoldiersTotalHP(Kingdom* k) {
    if (!k) return 0;
    int totalHP = 0;
    for (int i = 0; i < k->footSoldierCount; i++) { if (k->footSoldiers[i]) totalHP += k->footSoldiers[i]->hp; }
    return totalHP;
}

void WarHours::applyCasualties(Kingdom* target, int incomingDamage) {
    if (!target) return;
    
    // War Attrition Rule: opponents take strictly 5% of all pooled cyclic damage 
    int effectiveDmg = (int)(incomingDamage * 0.05f); 
    if (effectiveDmg <= 0) return;

    // 1. Cascade hits Cavalry
    for (int i = target->cavalryCount - 1; i >= 0 && effectiveDmg > 0; i--) {
        if (target->cavalry[i]) {
            int hp = target->cavalry[i]->hp;
            if (effectiveDmg >= hp) {
                effectiveDmg -= hp;
                // Engine owns memory. Null the pointer; Engine sweeps master array for nulls.
                target->cavalry[i] = nullptr;
                target->cavalryCount--;
            } else { target->cavalry[i]->hp -= effectiveDmg; effectiveDmg = 0; }
        }
    }

    // 2. Cascade spills to Knights
    for (int i = target->knightCount - 1; i >= 0 && effectiveDmg > 0; i--) {
        if (target->knights[i]) {
            int hp = target->knights[i]->hp;
            if (effectiveDmg >= hp) {
                effectiveDmg -= hp;
                // Engine owns memory. Null the pointer; Engine sweeps master array for nulls.
                target->knights[i] = nullptr;
                target->knightCount--;
            } else { target->knights[i]->hp -= effectiveDmg; effectiveDmg = 0; }
        }
    }

    // 3. Final remainder hits Infantry (re-activate incapacitated, count only)
    for (int i = target->footSoldierCount - 1; i >= 0 && effectiveDmg > 0; i--) {
        if (target->footSoldiers[i]) {
            int hp = target->footSoldiers[i]->hp;
            if (effectiveDmg >= hp) { effectiveDmg -= hp; target->footSoldiers[i]->hp = target->footSoldiers[i]->maxHp; target->footSoldierCount--; } 
            else { target->footSoldiers[i]->hp -= effectiveDmg; effectiveDmg = 0; }
        }
    }
}
    
int WarHours::resolveConflict() {
    // 0 Power Safety Check: PDF says war is aborted and relations reset to 0.
    // Return 2 to distinguish from stalemate (0) so caller can apply the correct relation reset.
    if (calculateTotalPower(attacker) == 0) return 2;
    
    while (currentHour < 24) {
        float mAtk = getStrategyModifier(attacker);
        float mDef = getStrategyModifier(defender);
        
        // Simultaneous generated outputs calculating exactly per phase
        int atkDmg = 0;
        for (int phase = 1; phase <= 4; phase++) atkDmg += generatePhaseDamage(attacker, mAtk, phase);
        
        int defDmg = 0;
        for (int phase = 1; phase <= 4; phase++) defDmg += generatePhaseDamage(defender, mDef, phase);
        
        // Approach 2: Observe before health snapshots
        int atkBeforeHealth = getFootSoldiersTotalHP(attacker);
        int defBeforeHealth = getFootSoldiersTotalHP(defender);
        
        // Physical casualties blindly apply cascading calculations
        applyCasualties(defender, atkDmg);
        applyCasualties(attacker, defDmg);
        
        // Observe outcome states
        int atkAfterHealth = getFootSoldiersTotalHP(attacker);
        int defAfterHealth = getFootSoldiersTotalHP(defender);
        
        // Calculates explicit damage differential ignoring all higher elite abstractions
        int damageTakenByFootsoldiersToHeal_Atk = atkBeforeHealth - atkAfterHealth;
        int damageTakenByFootsoldiersToHeal_Def = defBeforeHealth - defAfterHealth;
        
        // Hourly global Attrition triggers
        applyNavalAttrition(attacker);
        applyNavalAttrition(defender);
        
        // Triage safely triggers using purely observed mathematical bounds
        generateHealing(defender, damageTakenByFootsoldiersToHeal_Def);
        generateHealing(attacker, damageTakenByFootsoldiersToHeal_Atk);
        
        // --- Morale Output Validation ---
        int currentAtk = calculateTotalPower(attacker);
        int currentDef = calculateTotalPower(defender);
        
        // Rule: Output flags triggering routing aftermaths
        if (currentAtk < (int)(currentDef * 0.5f)) return 1;  
        if (currentDef < (int)(currentAtk * 0.5f)) return -1; 
        currentHour++;
    }
    
    return 0; // Forced Stalemate triggered.
}

void WarHours::applyWarAftermath(Kingdom* attacker, Kingdom* defender, int result, int relations[10][10]) {
    if (!attacker || !defender) return;

    int atkIdx = attacker->realmID;
    int defIdx = defender->realmID;

    // 1. Wealth Seizure (Conqueror cuts 75% of loser's wealth)
    if (result == -1) { // Attacker Wins (Conqueror)
        int loot = (int)(defender->wealth * 0.75f);
        attacker->wealth += loot;
        defender->wealth -= loot;
        if (defender->wealth < 0) defender->wealth = 0;
    }
    // (PDF doesn't explicitly detail a loot phase for Defender if they win by Rout, 
    // but implies "Conquerorship" applies if the attacker was the aggressor)

    // 2. Relation Resets (Table III)
    if (result == -1) { // Defender Breaks (Attacker Wins)
        relations[atkIdx][defIdx] = 0;
        relations[defIdx][atkIdx] = -50;
    } else { // Attacker Routs (1), Stalemate (0), or Abort (2)
        relations[atkIdx][defIdx] = 0;
        relations[defIdx][defIdx] = 0; 
        relations[defIdx][atkIdx] = 0;
    }

    // 3. Global Warmonger Penalty (Section 2.2)
    // "All other realms decrement Relations[X][Attacker] by 2."
    for (int i = 0; i < 10; i++) {
        if (i != atkIdx) {
            relations[i][atkIdx] -= 2;
            if (relations[i][atkIdx] < -100) relations[i][atkIdx] = -100;
        }
    }
}

