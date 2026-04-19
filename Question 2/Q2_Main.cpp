#include "Q2_Submission.h"
///////////////////////////////////////////  
//////////////This file was supposed to be Classes.cpp//////  
///////////////////////////////////////////  
  

void Asset::takeDmg(int damage) {
  int dr = damage - armor;
  int loss = (dr > 0) ? dr : 0;
  hp -= loss;
  if (hp < 0) hp = 0;
}

void Asset::takedefaultDmg(int damage) {
  hp -= damage;
  if (hp < 0)
    hp = 0;
}

void FrostPeaksKingdom::applyColdWeather() {
  for (int i = 0; i < countoffootsoldiers; i++)
    if (footSoldiers[i])
      footSoldiers[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofknights; i++)
    if (knights[i])
      knights[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofcavalry; i++)
    if (cavalry[i])
      cavalry[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofwarriorlords; i++)
    if (warriorLords[i]) {
      Knight* kLord = (Knight*)warriorLords[i];
      kLord->takedefaultDmg(coldAttritionModifier);
    }
  for (int i = 0; i < countoframs; i++)
    if (rams[i])
      rams[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofcatapults; i++)
    if (catapults[i])
      catapults[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofships; i++)
    if (warShips[i]) {
      Transport* tShip = (Transport*)warShips[i];
      tShip->takedefaultDmg(coldAttritionModifier);
    }
}

Kingdom::~Kingdom() {
      delete[] footSoldiers; delete[] knights;
      delete[] cavalry;
      delete[] rams;
      delete[] catapults;
      delete[] warShips;
      delete[] courtiers; delete[] warriorLords;
      delete[] sages;
      delete[] heirIds;
}

void terminateLord(Noble *n) {
  if (!n) return;
  
  switch(n->getTypeCode()) {
    case 1:
        delete (LandedLord *)n;
        break;
    case 2:
        delete (LandlessLord *)n;
        break;
    case 3:
        delete (WarriorLord *)n;
        break;
    default:
        delete n;
        break;
  }
}  
///////////////////////////////////////////  
//////////////This file was supposed to be InevitableFate.cpp//////  
///////////////////////////////////////////  
  

void InevitableFate::updateLordsDaily(Kingdom** realms, int countofrealms, Kingdom** rebels, int countofrebels) {
    if (realms == nullptr) return;

    
    for (int r = 0; r < countofrealms; r++) {
        Kingdom* realm = realms[r];
        if (realm == nullptr) continue;


        for (int i = 0; i < realm->countofcourtiers; i++) { if (realm->courtiers[i]) realm->courtiers[i]->age++; }
        for (int i = 0; i < realm->countofwarriorlords; i++) { if (realm->warriorLords[i]) realm->warriorLords[i]->Noble::age++; }


        if (realm->currentLord != nullptr) {
            realm->currentLord->age++;
            if (realm->currentLord->age >= 70) {
                realm->currentLord->healthMeter += 30.0f;
            }

            if (realm->currentLord->healthMeter >= 100.0f) {
                std::cout << "death...: Ruler " << realm->currentLord->getName() << " of Realm " << realm->realmID << " has died of natural causes.\n";
                realm->currentLord->isAlive = false; 
                realm->currentLord = nullptr;
                resolveSuccession(realm);



            }
        }
    }
    
    if (rebels != nullptr) {
        for (int r = 0; r < countofrebels; r++) {
            Kingdom* realm = rebels[r];
            if (realm == nullptr) continue;

            for (int i = 0; i < realm->countofcourtiers; i++) { if (realm->courtiers[i]) realm->courtiers[i]->age++; }
            for (int i = 0; i < realm->countofwarriorlords; i++) { if (realm->warriorLords[i]) realm->warriorLords[i]->Noble::age++; }

            if (realm->currentLord != nullptr) {
                realm->currentLord->age++;
                if (realm->currentLord->age >= 70) {
                    realm->currentLord->healthMeter += 30.0f;
                }
                if (realm->currentLord->healthMeter >= 100.0f) {
                    std::cout << "death...: Ruler " << realm->currentLord->getName() << " of Rebel Faction " << realm->realmID << " has died of natural causes.\n";
                    realm->currentLord->isAlive = false;
                    realm->currentLord = nullptr;
                    resolveSuccession(realm);
                }
            }
        }
    }
}

bool InevitableFate::isBetterEntity(int ambitionA, int idA, const char* nameA, int realmA, int ambitionB, int idB, const char* nameB, int realmB) {
    if (ambitionA > ambitionB) return true;
    if (ambitionA < ambitionB) return false;


    if (idA < idB) return true;
    if (idA > idB) return false;


    if (isSmallerThan(nameA, nameB)) return true;
    if (isSmallerThan(nameB, nameA)) return false;


    return (realmA < realmB);
}

Noble* InevitableFate::findHighestAmbition(Kingdom* realm) {
    if (realm == nullptr) return nullptr;
    
    Noble* presentBest = nullptr;

    for (int j = 0; j < realm->countofwarriorlords; j++) {
        WarriorLord* w = realm->warriorLords[j];
        if (!w) continue;
        if (!presentBest || isBetterEntity(w->getAmbitionStat(), w->Noble::getID(), w->Noble::getName(), realm->realmID, presentBest->getAmbitionStat(), presentBest->getID(), presentBest->getName(), realm->realmID)) {
            presentBest = (Noble*)w;
        }
    }

    for (int i = 0; i < realm->countofcourtiers; i++) {
        LandlessLord* c = realm->courtiers[i];
        if (!c) continue;
        if (!presentBest || isBetterEntity(c->getAmbitionStat(), c->getID(), c->getName(), realm->realmID, presentBest->getAmbitionStat(), presentBest->getID(), presentBest->getName(), realm->realmID)) {
            presentBest = (Noble*)c;
        }
    }

    return presentBest;
}

void InevitableFate::resolveSuccession(Kingdom* realm) {
    if (realm == nullptr) return;

    Noble* winner = findHighestAmbition(realm);

    if (winner != nullptr) {
        if (realm->currentLord != nullptr) {
            std::cout << "death...: Ruler " << realm->currentLord->getName() << " of Realm " << realm->realmID << " was slain.\n";
            realm->currentLord->isAlive = false; 
            realm->currentLord = nullptr;
        }

        realm->currentLord = winner;
        std::cout << "succession...: " << winner->getName() << " has ascended to the throne of Realm " << realm->realmID << "!\n";

        bool foundAndRemoved = false;

        for (int j = 0; j < realm->countofwarriorlords && !foundAndRemoved; j++) {
            if (realm->warriorLords[j] == (WarriorLord*)winner) {
                for (int k = j; k < realm->countofwarriorlords - 1; k++) {
                    realm->warriorLords[k] = realm->warriorLords[k + 1];
                }
                realm->countofwarriorlords--;
                foundAndRemoved = true;
            }
        }

        for (int i = 0; i < realm->countofcourtiers && !foundAndRemoved; i++) {
            if (realm->courtiers[i] == (LandlessLord*)winner) {
                for (int k = i; k < realm->countofcourtiers - 1; k++) {
                    realm->courtiers[k] = realm->courtiers[k + 1];
                }
                realm->countofcourtiers--;
                foundAndRemoved = true;
            }
        }
    }
}

int InevitableFate::calculateTotalArmySize(Kingdom* realm) {
    if (realm == nullptr) return 0;
    
    int totalUnits = realm->countoffootsoldiers + realm->countofknights + realm->countofcavalry + realm->countoframs + realm->countofcatapults + realm->countofships + realm->countofwarriorlords;
    
    return totalUnits;
}

void InevitableFate::updateTensionDaily(Kingdom** realms, int countofrealms, Kingdom** rebels, int countofrebels) {
    for (int r = 0; r < countofrealms; r++) {
        if (realms[r] == nullptr) continue;
        
        float netTensionChange = 0.0f;
        

        if (realms[r]->wealth < 0) netTensionChange += 5.0f;




        if (realms[r]->currentLord == nullptr) netTensionChange += 30.0f;
        else if (realms[r]->currentLord->strategyStat > 70) netTensionChange -= 3.0f;

        if (calculateTotalArmySize(realms[r]) > 200) netTensionChange -= 1.0f;
        
        for (int c = 0; c < realms[r]->countofcourtiers; c++) { if (realms[r]->courtiers[c] != nullptr && realms[r]->courtiers[c]->ambitionStat > 50) netTensionChange += (realms[r]->courtiers[c]->ambitionStat / 10.0f); }
        
        realms[r]->civilWarTension += netTensionChange;
        if (realms[r]->civilWarTension < 0.0f) realms[r]->civilWarTension = 0.0f;




    }
    
    if (rebels != nullptr) {
        for (int r = 0; r < countofrebels; r++) {
            if (rebels[r] == nullptr) continue;
            
            float netTensionChange = 0.0f;
            
            if (rebels[r]->wealth < 0) netTensionChange += 5.0f;
            
            if (rebels[r]->currentLord == nullptr) netTensionChange += 30.0f;
            else if (rebels[r]->currentLord->strategyStat > 70) netTensionChange -= 3.0f;
            
            if (calculateTotalArmySize(rebels[r]) > 200) netTensionChange -= 1.0f;
            

            for (int c = 0; c < rebels[r]->countofcourtiers; c++) { if (rebels[r]->courtiers[c] != nullptr && rebels[r]->courtiers[c]->ambitionStat > 50) netTensionChange += (rebels[r]->courtiers[c]->ambitionStat / 10.0f); }
            
            
            rebels[r]->civilWarTension += netTensionChange;
            if (rebels[r]->civilWarTension < 0.0f) rebels[r]->civilWarTension = 0.0f;
        }
    }
}

void InevitableFate::updatePlotsDaily(Kingdom** realms, int countofrealms, int** relations, Kingdom** rebels, int countofrebels) {
    if (realms == nullptr) return;

    for (int r = 0; r < countofrealms; r++) {
        Kingdom* realm = realms[r];
        if (realm == nullptr) continue;

        for (int i = 0; i < realm->countofcourtiers; i++) {
            LandlessLord* plotter = realm->courtiers[i];
            if (!plotter || plotter->ambitionStat <= 60) continue;

            float tick = (plotter->ambitionStat - 60) / 5.0f;

            bool hasEnemies = false;
            for (int rival = 0; rival < countofrealms; rival++) {
                if (relations[r][rival] < 0) {
                    hasEnemies = true;
                    break;
                }
            }
            
            
            if (hasEnemies) tick += 2.0f;

            
            
            if (realm->currentLord) {
                tick -= (realm->currentLord->strategyStat / 25.0f);
            }

            if (realm->defenseStat > 50) {
                tick -= 1.0f;
            }

            if (tick < 0.0f) tick = 0.0f;

            
            plotter->assassinationPlotProgress += tick;
            
            if (plotter->assassinationPlotProgress > 100.0f) plotter->assassinationPlotProgress = 100.0f;
            



        }
    }
    
    if (rebels != nullptr) {
        for (int r = 0; r < countofrebels; r++) {
            Kingdom* realm = rebels[r];
            if (realm == nullptr) continue;

            for (int i = 0; i < realm->countofcourtiers; i++) {
                LandlessLord* plotter = realm->courtiers[i];
                if (!plotter || plotter->ambitionStat <= 60) continue;

                float tick = (plotter->ambitionStat - 60) / 5.0f;

                bool hasEnemies = false;
                for (int rival = 0; rival < countofrealms; rival++) {
                    if (relations[realm->realmID][rival] < 0) {
                        hasEnemies = true; break;
                    }
                }
                if (!hasEnemies) {
                    for (int rival = 0; rival < countofrebels; rival++) {
                        int rID = countofrealms + rival;
                        if (realm->realmID != rID && relations[realm->realmID][rID] < 0) {
                            hasEnemies = true; break;
                        }
                    }
                }
                
                if (hasEnemies) tick += 2.0f;

                if (realm->currentLord) {
                    tick -= (realm->currentLord->strategyStat / 25.0f);
                }

                if (realm->defenseStat > 50) {
                    tick -= 1.0f;
                }

                if (tick < 0.0f) tick = 0.0f;

                plotter->assassinationPlotProgress += tick;
                if (plotter->assassinationPlotProgress > 100.0f) plotter->assassinationPlotProgress = 100.0f;
            }
        }

    }


}

void InevitableFate::resolveInternalCoup(Kingdom* realm, LandlessLord* plotter, int** relations, int countofrealms) {
    if (!realm || !plotter || !realm->currentLord) return;

    float power = plotter->ambitionStat + (realm->wealth / 100.0f);

    float ageMod = (realm->currentLord->age < 40) ? 5.0f : -5.0f;
    float defense = realm->currentLord->strategyStat + (realm->defenseStat / 10.0f) + ageMod;

    std::cout << "plot: " << plotter->getName() << " executes an Internal Coup in Realm " << realm->realmID << "!\n";

    if (power > defense) {
        
        resolveSuccession(realm);
    } else {
        std::cout << "death: plotter " << plotter->getName() << " was executed for treason in Realm " << realm->realmID << ".\n";
        plotter->isAlive = false; 
        
        for (int j = 0; j < countofrealms; j++) {
            if (j != realm->realmID) {
                relations[realm->realmID][j] -= 30;
                relations[j][realm->realmID] -= 30; 
            }
        }
    }

    plotter->assassinationPlotProgress = 0.0f;
}

void InevitableFate::resolveForeignStrike(Kingdom** realms, int countofrealms, Assassin** assassins, int& aCount, int** relations) {
    if (!realms || !assassins) return;

    for (int i = 0; i < aCount; i++) {
        Assassin* agent = assassins[i];
        if (!agent || !agent->isAlive) continue;

        int clientIdx = agent->clientIdx;
        Kingdom* clientRealm = realms[clientIdx];
        if (!clientRealm) continue;

        LandlessLord* plotter = nullptr;
        for (int p = 0; p < clientRealm->countofcourtiers; p++) {
            if (clientRealm->courtiers[p] && clientRealm->courtiers[p]->assassinationPlotProgress >= 100.0f) {
                plotter = clientRealm->courtiers[p];
                break;
            }
        }
        if (!plotter) continue;

        int targetIdx = -1;
        int minRelation = 0;
        for (int r = 0; r < countofrealms; r++) {
            if (r != clientIdx && relations[clientIdx][r] <= -50) {
                if (targetIdx == -1 || relations[clientIdx][r] < minRelation) {
                    minRelation = relations[clientIdx][r];
                    targetIdx = r;
                }
            }
        }
        
        if (targetIdx == -1) continue;

        Kingdom* targetRealm = realms[targetIdx];
        if (!targetRealm || !targetRealm->currentLord) continue;

        std::cout << "assassination  : Foreign Strike executed by Assassin " << agent->getName() << " against Realm " << targetIdx << "!\n";

        if (agent->stealthStat > targetRealm->currentLord->strategyStat) {
            resolveSuccession(targetRealm);
        } else {
            std::cout << "death : Assassin " << agent->getName() << " was caught and executed.\n";
            relations[targetIdx][clientIdx] -= 20;
        }

        agent->isAlive = false;
        
        plotter->assassinationPlotProgress = 0.0f;
        
    }
}
  
///////////////////////////////////////////  
//////////////This file was supposed to be WarHours.cpp//////  
///////////////////////////////////////////  
  

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
  
///////////////////////////////////////////  
//////////////This file was supposed to be ScenarioLoader.cpp//////  
///////////////////////////////////////////  
  
#include <iostream>
#include <fstream>

bool ScenarioLoader::load(const char* csvPath, Aethelgard* engine) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open scenario file: " << csvPath << "\n";
        return false;
    }

    char line[512];
    int section = 0; 
    int Row_rel = 0;  

    while (file.getline(line, 512)) {
        if (line[0] == '\0' || line[0] == '#') continue;

        if (findSubstring(line, "[KINGDOMS]") != -1) { section = 1; continue; }
        if (findSubstring(line, "[LORDS]") != -1) { section = 2; continue; }
        if (findSubstring(line, "[RELATIONS]") != -1)  { section = 3; continue; }
        if (findSubstring(line, "[MILITARY]") != -1) { section = 4; continue; }
        if (findSubstring(line, "[SAGES]") != -1) { section = 5; continue; }
        if (findSubstring(line, "[ASSASSINS]") != -1){ section = 6; continue; }
        if (findSubstring(line, "[CONFIG]") != -1) { section = 7; continue; }

        char* toks[110];
        int count_toks = 0;
        char* present = line;

        if (section == 3) {
            while (*present != '\0' && count_toks < 20) {
                while (*present == ' ' || *present == '\t') present++;
                if (*present == '\0') break;
                toks[count_toks++] = present;
                while (*present != ' ' && *present != '\t' && *present != '\0') present++;
                if (*present != '\0') { *present = '\0'; present++; }
            }
        } 
        else {
            while (present && count_toks < 105) {
                toks[count_toks++] = present;
                int index_Comman = findFirstOccurrence(present, ',');
                if (index_Comman != -1) {
                    present[index_Comman] = '\0';
                    present = present + index_Comman + 1;
                } else {
                    present = nullptr;
                }
            }
        }

        if (count_toks < 2) continue; 
        if (findSubstring(toks[0], "ID") != -1 || findSubstring(toks[0], "RealmID") != -1 || 
            findSubstring(toks[0], "LordID") != -1 || findSubstring(toks[0], "AssassinID") != -1) continue;
        if (findSubstring(toks[1], "Name") != -1 || findSubstring(toks[1], "RealmName") != -1) continue;
        if (toks[0][0] == '\0' && (count_toks < 3 || toks[1][0] == '\0')) continue;

        if (count_toks == 0) continue;

        if (section == 1) { 
            int id = stoi(toks[0]);
            int w = stoi(toks[2]);
            int t = stoi(toks[3]);
            int d = stoi(toks[4]);
            
            if (findSubstring(toks[1], "Void Rift") != -1) {
                TheVoidRift* vr = new TheVoidRift();
                engine->setVoidRift(vr);
                // Ensure countofrealms accounts for the Void Rift's index
                if (id >= 0 && id < 100) {
                    Kingdom* dummy = nullptr; // We'll just trigger the counter update
                    engine->addRealm(nullptr, id); 
                }
            } else {
                Kingdom* k = nullptr;
                int extra = (count_toks > 6) ? stoi(toks[6]) : 10; 
                if (findSubstring(toks[5], "Coastal") != -1) k = new CoastalKingdom(w, d, id, t, extra); 
                else if (findSubstring(toks[5], "Frozen") != -1) k = new FrostPeaksKingdom(w, d, id, t);
                else k = new VerdantKingdom(w, d, id, t, extra); 
                
                if (k) {
                    engine->addRealm(k, id);
                }
            }
        }
        else if (section == 2) { 
            int id = stoi(toks[0]);
            int realmId = stoi(toks[5]);
            int typeIdx = (findSubstring(toks[4], "Landed") != -1) ? 1 : 
                          (findSubstring(toks[4], "Landless") != -1) ? 2 : 3;

            Noble* n = nullptr;
            if (typeIdx == 1) n = new LandedLord(copyString(toks[1]), id, stoi(toks[2]), 0, stoi(toks[9]), stoi(toks[7]), stoi(toks[6]), stoi(toks[8]), realmId, nullptr);
            else if (typeIdx == 2) n = new LandlessLord(copyString(toks[1]), id, stoi(toks[2]), 0, stoi(toks[9]), stoi(toks[7]), stoi(toks[6]), stoi(toks[8]), realmId, nullptr);
            else n = new WarriorLord(copyString(toks[1]), id, stoi(toks[2]), 0, stoi(toks[9]), stoi(toks[6]), stoi(toks[7]), stoi(toks[8]), realmId, nullptr, 100, 100, 20, 5, 60, true, true);

            if (n) {
                engine->addLord(n);
                Kingdom* target = engine->getRealm(realmId);
                if (target) {
                    if (typeIdx == 1) target->setCurrentLord(n);
                    else if (typeIdx == 2) target->addCourtier((LandlessLord*)n);
                    else if (typeIdx == 3) target->addWarriorLord((WarriorLord*)n);
                }
            }
        }
        else if (section == 3) { 
            if (Row_rel < 100) {
                for (int c = 0; c < count_toks && c < 100; c++) {
                    engine->setRelation(Row_rel, c, stoi(toks[c]));
                }
                Row_rel++;
            }
        }
        else if (section == 4) { 
            int rid = stoi(toks[0]);
            int count = stoi(toks[2]);
            Kingdom* target = engine->getRealm(rid);
            if (target) {
                for (int i = 0; i < count; i++) {
                    if (findSubstring(toks[1], "Knight") != -1) {
                        Knight* kn = new Knight(copyString(toks[1]), 1000+rid*100+i, stoi(toks[4]), stoi(toks[4]), 20, 5, stoi(toks[3]), true, true);
                        engine->keepTrackOfKnights(kn);
                        target->addKnight(kn);
                    } else if (findSubstring(toks[1], "Foot") != -1) {
                        FootSoldier* fs = new FootSoldier(copyString(toks[1]), 2000+rid*100+i, stoi(toks[4]), stoi(toks[4]), 10, 2, stoi(toks[3]), true);
                        engine->keepTrackOfFoot(fs);
                        target->addFootSoldier(fs);
                    } else if (findSubstring(toks[1], "Cavalry") != -1) {
                        Cavalry* cv = new Cavalry(copyString(toks[1]), 3000+rid*100+i, stoi(toks[4]), stoi(toks[4]), 15, 6, stoi(toks[3]), 2.0f, 1.0f);
                        engine->keepTrackOfCavalry(cv);
                        target->addCavalry(cv);
                    } else if (findSubstring(toks[1], "Ram") != -1) {
                        BatteringRam* br = new BatteringRam(copyString(toks[1]), 4000+rid*100+i, stoi(toks[4]), stoi(toks[4]), 40, 10, stoi(toks[5]), 1);
                        engine->keepTrackOfRams(br);
                        target->addRam(br);
                    } else if (findSubstring(toks[1], "Catapult") != -1) {
                        Catapult* ct = new Catapult(copyString(toks[1]), 5000+rid*100+i, stoi(toks[4]), stoi(toks[4]), 25, 8, stoi(toks[5]), 1, 10, stoi(toks[6]));
                        engine->keepTrackOfCatapults(ct);
                        target->addCatapult(ct);
                    } else if (findSubstring(toks[1], "Ship") != -1) {
                        WarShip* ws = new WarShip(6000+rid*100+i, copyString(toks[1]), stoi(toks[4]), stoi(toks[4]), 50, 15, 100, 10, stoi(toks[5]), 5);
                        engine->keepTrackOfShips(ws);
                        target->addWarShip(ws);
                    }
                }
            }
        }
        else if (section == 5) { 
            int rid = stoi(toks[0]);
            Kingdom* target = engine->getRealm(rid);
            if (target) {
                target->addSage(new Sage(copyString(toks[1]), 3000+rid, 50, stoi(toks[2]), stoi(toks[3])));
            }
        }
        else if (section == 6) { 
            engine->addAssassin(new Assassin(copyString(toks[1]), stoi(toks[0]), stoi(toks[2]), stoi(toks[3]), stoi(toks[4])));
        }
        else if (section == 7) { 
            if (findSubstring(line, "ThreatMax") != -1) {
                int eqIdx = findFirstOccurrence(line, '=');
                if (eqIdx != -1) {
                    engine->setThreatMax(stoi(line + eqIdx + 1));
                }
            }
        }
    }
    return true;
}
  
///////////////////////////////////////////  
//////////////This file was supposed to be Aethelgard.cpp//////  
///////////////////////////////////////////  
  
#include <iostream>





Aethelgard::Aethelgard() : countofrealms(0), voidRift(nullptr),allthelords(nullptr), countoflords(0),assassins(nullptr), assassinCount(0),therebellingfactions(nullptr), countofrebels(0),allFootSoldiers(nullptr), countoffootsoldiers(0),allKnights(nullptr), countofknights(0),allCavalry(nullptr), countofcavalry(0),allRams(nullptr), countoframs(0),allCatapults(nullptr), countofcatapults(0),allWarShips(nullptr), countofships(0),allWarriorLords(nullptr), countofwarriorlords(0),relationDimension(100),currentDay(0), threatMax(60){
    
    
    for (int i = 0; i < 100; i++) realms[i] = nullptr;


    relations = new int*[relationDimension];
    for (int i = 0; i < relationDimension; i++) {

        relations[i] = new int[relationDimension];

        for (int j = 0; j < relationDimension; j++)
            relations[i][j] = (i == j) ? 0 : 50;

    }
}

Aethelgard::~Aethelgard() {
    for (int i = 0; i < countoflords; i++) {

        if (allthelords[i]) {

            Noble* n = allthelords[i];
            int tc = n->getTypeCode();
            if (tc == 1) delete (LandedLord*)n;
            else if (tc == 2) delete (LandlessLord*)n;
            else if (tc == 3) delete (WarriorLord*)n;
            else delete n;
            allthelords[i] = nullptr;
        }
    }
    delete[] allthelords;


    for (int i = 0; i < assassinCount; i++) {

        delete assassins[i]; assassins[i] = nullptr;


    }

    delete[] assassins;

    for (int i = 0; i < countoffootsoldiers; i++) delete allFootSoldiers[i]; 
    delete[] allFootSoldiers;

    for (int i = 0; i < countofknights; i++) delete allKnights[i];   
    delete[] allKnights;

    for (int i = 0; i < countofcavalry; i++) delete allCavalry[i];   
    delete[] allCavalry;

    for (int i = 0; i < countoframs; i++) delete allRams[i];      
    delete[] allRams;


    for (int i = 0; i < countofcatapults; i++) delete allCatapults[i]; 
    delete[] allCatapults;


    for (int i = 0; i < countofships; i++)delete allWarShips[i];  
    delete[] allWarShips;


    for (int i = 0; i < countofwarriorlords; i++) {

        allWarriorLords[i] = nullptr; 
    }
    delete[] allWarriorLords;

    for (int i = 0; i < countofrealms; i++) {

        if (realms[i] ) {

            int code = realms[i]->getTerrainCode();
            if (code == 1) delete (FrostPeaksKingdom*)realms[i];
            else if (code == 2) delete (VerdantKingdom*)realms[i];
            else if (code == 3) delete (CoastalKingdom*)realms[i];

            else if (code == 4) delete (TheVoidRift*)realms[i];
            else delete realms[i];

            realms[i] = nullptr;
        }
    }
    for (int i = 0; i < countofrebels; i++) {
        if (therebellingfactions[i]) {
            int code = therebellingfactions[i]->getTerrainCode();
            
            if (code == 1) delete (FrostPeaksKingdom*)therebellingfactions[i];
            else if (code == 2) delete (VerdantKingdom*)therebellingfactions[i];
            else if (code == 3) delete (CoastalKingdom*)therebellingfactions[i];
            else delete therebellingfactions[i];
        }
    }
    delete[] therebellingfactions;

    for (int i = 0; i < relationDimension; i++) delete[] relations[i];
    delete[] relations;

}

void Aethelgard::addRealm(Kingdom* k, int idx) {
    if (idx < 0 || idx >= 100) return;
    realms[idx] = k;
    if (idx >= countofrealms) countofrealms = idx + 1;
}

void Aethelgard::addLord(Noble* n) {
    Noble** bigger = new Noble*[countoflords + 1];
    for (int i = 0; i < countoflords; i++) bigger[i] = allthelords[i];

    bigger[countoflords] = n;

    delete[] allthelords;
    
    allthelords = bigger;
    countoflords++;
}

void Aethelgard::addAssassin(Assassin* a) {

    Assassin** bigger = new Assassin*[assassinCount + 1];

    for (int i = 0; i < assassinCount; i++) bigger[i] = assassins[i];

    bigger[assassinCount] = a;
    delete[] assassins;
    assassins = bigger;
    assassinCount++;
}

void Aethelgard::setRelation(int i, int j, int val) {
    if (i >= 0 && i < relationDimension && j >= 0 && j < relationDimension)
        relations[i][j] = val;
}

void Aethelgard::keepTrackOfFoot(FootSoldier* f) {
    FootSoldier** t = new FootSoldier*[countoffootsoldiers + 1];
    for(int i=0;i<countoffootsoldiers;i++) t[i] = allFootSoldiers[i]; 
    t[countoffootsoldiers++] = f; 
    delete[] allFootSoldiers; 
    allFootSoldiers = t;
 }
void Aethelgard::keepTrackOfKnights(Knight* k) { 
    Knight** t = new Knight*[countofknights + 1]; 
    for(int i=0;i<countofknights;i++) t[i] = allKnights[i]; 
    t[countofknights++] = k; 
    delete[] allKnights; 
    allKnights = t;
}
void Aethelgard::keepTrackOfCavalry(Cavalry* c) { 
    Cavalry** t = new Cavalry*[countofcavalry + 1]; 
    for(int i=0;i<countofcavalry;i++) t[i] = allCavalry[i]; 
    t[countofcavalry++] = c; 
    delete[] allCavalry; 
    allCavalry = t; 
}
void Aethelgard::keepTrackOfRams(BatteringRam* r) { 
    BatteringRam** t = new BatteringRam*[countoframs + 1]; 
    for(int i=0;i<countoframs;i++) t[i] = allRams[i]; 
    t[countoframs++] = r; 
    delete[] allRams; 
    allRams = t; 
}
void Aethelgard::keepTrackOfCatapults(Catapult* c) { 
    Catapult** t = new Catapult*[countofcatapults + 1]; 
    for(int i=0;i<countofcatapults;i++) t[i] = allCatapults[i]; 
    t[countofcatapults++] = c; 
    delete[] allCatapults; 
    allCatapults = t; 
}
void Aethelgard::keepTrackOfShips(WarShip* w) { 
    WarShip** t = new WarShip*[countofships + 1]; 
    for(int i=0;i<countofships;i++) t[i] = allWarShips[i]; 
    t[countofships++] = w; 
    delete[] allWarShips; 
    allWarShips = t; 
}

void Aethelgard::keepTrackOfWarriorLords(WarriorLord* w) { 
    WarriorLord** t = new WarriorLord*[countofwarriorlords + 1]; 
    for(int i=0;i<countofwarriorlords;i++) t[i] = allWarriorLords[i]; 
    t[countofwarriorlords++] = w; 
    delete[] allWarriorLords;
     allWarriorLords = t; 
    }


void Aethelgard::setThreatMax(int t) { 
    threatMax = t;
 }


void Aethelgard::setVoidRift(TheVoidRift* vr) { 
    voidRift = vr; 
}


void Aethelgard::doShadyStuff() {
    fate.updatePlotsDaily(realms, countofrealms, relations, therebellingfactions, countofrebels);

    int totalCount = countofrealms + countofrebels;

    for (int r = 0; r < totalCount; r++) {
        Kingdom* k = (r < countofrealms) ? realms[r] : therebellingfactions[r - countofrealms];
    
        if (!k) continue;
    
        for (int c = 0; c < k->getCourtierCount(); c++) {
    
            LandlessLord* plotter = k->getCourtier(c);
    
            if (plotter && plotter->getPlotProgress() >= 100.0f) {
                bool hasForeignTarget = false;
    
                for (int j = 0; j < totalCount; j++) {
                    if (j != r && relations[r][j] <= -50)
                    { hasForeignTarget = true; break; }
                }

                if (hasForeignTarget) {
                    fate.resolveForeignStrike(realms, countofrealms, assassins, assassinCount, relations); 
    
                }
                else {

                    fate.resolveInternalCoup(k, plotter, relations, totalCount);
                }
                break; 
            }

        }
    }

}

void Aethelgard::makePeopleHateEachOther() {
    
    int totalCount = countofrealms + countofrebels;
    
    for (int i = 0; i < totalCount; i++) {
    
        for (int j = 0; j < totalCount; j++) {
    
            if (i == j) continue;
    
            if (relations[i][j] > -100) {
                relations[i][j]--;
            }
        }
    }
}

void Aethelgard::handleTheFighting() {
    
    int atack_index = -1, defend_index = -1;
    int lowestRelation = -80; 
    int totalCount = countofrealms + countofrebels;

    for (int i = 0; i < totalCount; i++) {
    
        Kingdom* iRealm = (i < countofrealms) ? realms[i] : therebellingfactions[i - countofrealms];
        if (!iRealm || !iRealm->getCurrentLord()) continue;
        for (int j = 0; j < totalCount; j++) {
    
            Kingdom* jRealm = (j < countofrealms) ? realms[j] : therebellingfactions[j - countofrealms];
    
            if (i == j || !jRealm || !jRealm->getCurrentLord()) continue;
    
            if (relations[i][j] < lowestRelation) {
                lowestRelation = relations[i][j];
                atack_index = i;
    
                defend_index = j;
    
            }
        }
    }

    if (atack_index == -1) return; 

    Kingdom* atk = (atack_index < countofrealms) ? realms[atack_index] : therebellingfactions[atack_index - countofrealms];
    Kingdom* def = (defend_index < countofrealms) ? realms[defend_index] : therebellingfactions[defend_index - countofrealms];

    std::cout << "\nDay " << currentDay << ": ###### WAR HAS BEEN DECLARED ######\n";
    std::cout << "Attacker: Realm " << atack_index << "  ###  Defender: Realm " << defend_index << "\n";

    WarHours war(atk, def);
    int result = war.figureOutWhoWins();

    war.doPostWarStuff(atk, def, result, relations, totalCount);

    const char* outcome = (result == -1) ? "CONQUERED" :(result ==  1) ? "REPELLED" : (result ==  0) ? "STALEMATE" : "ABORTED";
    std::cout << "Day " << currentDay << ": Battle outcome: " << outcome << "\n";
}

void Aethelgard::removeTheDead() {
    for (int i = 0; i < countoflords; i++) {

        if (allthelords[i] && !allthelords[i]->isAliveSelf()) {
            // Fix dangling currentLord pointer in Kingdom
            int rIdx = allthelords[i]->realmIdx;
            if (rIdx >= 0) {
                Kingdom* k = (rIdx < countofrealms) ? realms[rIdx] : ((rIdx - countofrealms < countofrebels) ? therebellingfactions[rIdx - countofrealms] : nullptr);
                if (k && k->currentLord == allthelords[i]) k->currentLord = nullptr;
            }
            terminateLord(allthelords[i]);
            allthelords[i] = nullptr;

        }
    }
    for (int i = 0; i < assassinCount; i++) {
        if (assassins[i] && !assassins[i]->isAliveSelf()) {
            
            delete assassins[i];
            assassins[i] = nullptr;
        }
    }

}



void Aethelgard::makeARebellion(int parentIdx) {
    if (parentIdx < 0 || parentIdx >= countofrealms) return;
    Kingdom* parent = realms[parentIdx];
    if (!parent) return;

    LandlessLord* rebel = nullptr;
    int maxAmb = -1;


    for (int c = 0; c < parent->getCourtierCount(); c++) {
        LandlessLord* crt = parent->getCourtier(c);
        if (!crt) continue;
        if (crt->getAmbitionStat() > maxAmb ||
           (crt->getAmbitionStat() == maxAmb && rebel && isSmallerThan(crt->getName(), rebel->getName()))) {
            maxAmb = crt->getAmbitionStat();
            rebel = crt;
        }
    }


    if (!rebel) return; 

    int newIdx = countofrealms + countofrebels;
    

    bool foundAndRemoved = false;
    for (int i = 0; i < parent->countofcourtiers && !foundAndRemoved; i++) {
        if (parent->courtiers[i] == rebel) {
            for (int k = i; k < parent->countofcourtiers - 1; k++) {
                parent->courtiers[k] = parent->courtiers[k + 1];
            }
            parent->countofcourtiers--;
            foundAndRemoved = true;
        }
    }
    


    Kingdom* rebelRealm = new Kingdom(*parent, rebel, newIdx);

    Kingdom** bigger = new Kingdom*[countofrebels + 1];
    for (int i = 0; i < countofrebels; i++) bigger[i] = therebellingfactions[i];
    bigger[countofrebels] = rebelRealm;
    delete[] therebellingfactions;
    
    
    therebellingfactions = bigger;
    countofrebels++;

    setRelation(parentIdx, newIdx, -100);
    setRelation(newIdx, parentIdx, -100);

    parent->resetTension();

    std::cout << "The Realm " << parentIdx << " got broken and Rebel faction was born at index " << newIdx << ".\n";
}



bool Aethelgard::loadScenario(const char* csvPath) {
    return ScenarioLoader::load(csvPath, this);
}



void Aethelgard::showWhatsHappening() {
    std::cout << "\n\nDay " << currentDay << ": THE SCHOLAR'S DASHBOARD\n\n";

    int tLvl = voidRift ? voidRift->getThreatLevel() : 0;
    std::cout << "Invasion Progress: " << tLvl << " out of " << threatMax << "\n\n";

    std::cout << "\n\nTHE REALM LEDGER\n\n";

    int totalCount = countofrealms + countofrebels;

    for (int r = 0; r < totalCount; r++) {

        Kingdom* k = (r < countofrealms) ? realms[r] : therebellingfactions[r - countofrealms];

        if (!k || k->getTerrainCode() == 4) continue; 

        int power = (k->countoffootsoldiers * 1) + (k->countofcavalry * 5) + (k->countofknights * 8) + (k->countofships * 12) + (k->countoframs * 25) + (k->countofcatapults * 30) + (k->countofwarriorlords * 10);

        const char* lordName = k->getCurrentLord() ? k->getCurrentLord()->getName() : "None";

        std::cout << "Realm " << k->getRealmID() << "   Wealth: " << k->getWealth() << "   Power: " << power << "   Ruler: " << lordName << "\n";
    }

    std::cout << "\n\nTHE DIPLOMATIC WEB\n\n";
    for (int i = 0; i < countofrealms; i++) {
        for (int j = 0; j < countofrealms; j++) {
            if (relations[i][j] >= 0 && relations[i][j] < 10) std::cout << "      " << relations[i][j];
            else if (relations[i][j] > -10 && relations[i][j] < 100) std::cout << "     " << relations[i][j];
            else std::cout << "    " << relations[i][j];
        }
        std::cout << "\n";
    }

    std::cout << "\n\nTHE WHISPERING COUNCIL\n\n";
    bool anyPlots = false;
    for (int r = 0; r < countofrealms; r++) {

        if (!realms[r]) continue;

        for (int c = 0; c < realms[r]->getCourtierCount(); c++) {

            LandlessLord* plotter = realms[r]->getCourtier(c);
            if (plotter && plotter->getPlotProgress() > 5.0f) {

                std::cout << "PLOT DETECTED: " << plotter->getName() 
                          << " (Realm " << r << ") = Progress: " << plotter->getPlotProgress() << "%\n";
                anyPlots = true;
            }
        }
    }



    for (int i = 0; i < assassinCount; i++) {
        if (assassins[i] && assassins[i]->isAliveSelf()) {
            std::cout << "ASSASSIN ACTIVE: " << assassins[i]->getName() << " (Client: Realm " << assassins[i]->clientIdx << ")\n";
            anyPlots = true;
        }
    }


    
    if (!anyPlots) std::cout << "All is quiet in the shadows.\n";

    std::cout << "\n\n";
}

void Aethelgard::run(const char* csvPath, int days) {

    if (!loadScenario(csvPath)) {
        std::cerr << "Simulation aborted: Load failed.\n";
        return;
    }

    std::cout << "\n=== AETHELGARD SIMULATION SHALL NOWW BEGIN ((Days: " << days << ") ===\n";

    for (int d = 0; d < days; d++) {

        currentDay = d + 1;
        showWhatsHappening();

        for (int r = 0; r < countofrealms; r++) {
            if (realms[r]) {
                int code = realms[r]->getTerrainCode();
                if (code == 1)((FrostPeaksKingdom*)realms[r])->performAction();
                else if (code == 2) ((VerdantKingdom*)realms[r])->performAction();
                else if (code == 3) ((CoastalKingdom*)realms[r])->performAction();
                else if (code == 4) ((TheVoidRift*)realms[r])->performAction();
                else realms[r]->performAction();
            }
        }
        for (int r = 0; r < countofrebels; r++) {
            if (therebellingfactions[r]) {
                int code = therebellingfactions[r]->getTerrainCode();
                if (code == 1) ((FrostPeaksKingdom*)therebellingfactions[r])->performAction();
                else if (code == 2) ((VerdantKingdom*)therebellingfactions[r])->performAction();
                else if (code == 3) ((CoastalKingdom*)therebellingfactions[r])->performAction();
                else if (code == 4) ((TheVoidRift*)therebellingfactions[r])->performAction();
                else therebellingfactions[r]->performAction();
            }
        }

        fate.updateLordsDaily(realms, countofrealms, therebellingfactions, countofrebels);
        fate.updateTensionDaily(realms, countofrealms, therebellingfactions, countofrebels);


        doShadyStuff();
        makePeopleHateEachOther();
        handleTheFighting();

        if (voidRift) {
            
            voidRift->performAction();

            if (voidRift->getThreatLevel() >= threatMax) {

                int globalPower = 0;
                for (int r = 0; r < countofrealms; r++) {
                    if (realms[r] && realms[r]->getTerrainCode() != 4) {
                        globalPower += (realms[r]->countoffootsoldiers * 1) + (realms[r]->countofcavalry * 5) + (realms[r]->countofknights * 8) + (realms[r]->countofships * 12) + (realms[r]->countoframs * 25) + (realms[r]->countofwarriorlords * 10);
                    }
                }


                for (int r = 0; r < countofrebels; r++) {
                    if (therebellingfactions[r]) {
                        globalPower += (therebellingfactions[r]->countoffootsoldiers * 1) + (therebellingfactions[r]->countofcavalry * 5) + (therebellingfactions[r]->countofknights * 8) + (therebellingfactions[r]->countofships * 12) + (therebellingfactions[r]->countoframs * 25) + (therebellingfactions[r]->countofwarriorlords * 10);
                    }
                }


                int voidPower = voidRift->getThreatLevel() * 100;

                std::cout << "\nDay " << currentDay << ": ###--ECLIPSE--### The Void Rift has erupted! (Threat Power: " << voidPower << " vs Alliance Power: " << globalPower << ")\n";

                if (globalPower >= voidPower) {

                    std::cout << "  ===>>>  Aethelgard was able to  repel the attack... but faces a 60%  casualty rate.\n";

                    for (int r = 0; r < countofrealms; r++) {
                        if (realms[r] && realms[r]->getTerrainCode() != 4) {
                
                            realms[r]->countoffootsoldiers = (int)(realms[r]->countoffootsoldiers * 0.4f);
                            realms[r]->countofcavalry = (int)(realms[r]->countofcavalry * 0.4f);
                            realms[r]->countofknights = (int)(realms[r]->countofknights * 0.4f);
                            realms[r]->countoframs = (int)(realms[r]->countoframs * 0.4f);
                            realms[r]->countofcatapults = (int)(realms[r]->countofcatapults * 0.4f);
                            realms[r]->countofships = (int)(realms[r]->countofships * 0.4f);
                            realms[r]->countofwarriorlords = (int)(realms[r]->countofwarriorlords * 0.4f);

                        }
                    }
                    for (int r = 0; r < countofrebels; r++) {

                        if (therebellingfactions[r]) {

                            therebellingfactions[r]->countoffootsoldiers = (int)(therebellingfactions[r]->countoffootsoldiers * 0.4f);
                            therebellingfactions[r]->countofcavalry = (int)(therebellingfactions[r]->countofcavalry * 0.4f);
                            therebellingfactions[r]->countofknights = (int)(therebellingfactions[r]->countofknights * 0.4f);
                            therebellingfactions[r]->countoframs = (int)(therebellingfactions[r]->countoframs * 0.4f);
                            therebellingfactions[r]->countofcatapults = (int)(therebellingfactions[r]->countofcatapults * 0.4f);
                            therebellingfactions[r]->countofships = (int)(therebellingfactions[r]->countofships * 0.4f);
                            therebellingfactions[r]->countofwarriorlords = (int)(therebellingfactions[r]->countofwarriorlords * 0.4f);

                        }
                    }
                } else {

                    std::cout << "  ===>>> AETHELGARD HAS BEEN CONSUMED. OMG an Absolute defeat.\n";

                }
                break;
            }
        }

        removeTheDead();

        for (int r = 0; r < countofrealms; r++) {
            if (realms[r] && realms[r]->getTension() >= 100.0f) {
                makeARebellion(r);
            }
        }
    }

    std::cout << "\n### SIMULATION COMPLETE (Day " << currentDay << ") ###\n";
}
  
///////////////////////////////////////////  
//////////////This file was supposed to be main.cpp//////  
///////////////////////////////////////////  
  
#include <iostream>

using namespace std;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <scenario_csv> [days]\n";

        return 1;
    }

    const char* csvPath = argv[1];
    int days = (argc >= 3) ? stoi(argv[2]) : 5;

    Aethelgard engine;
    engine.run(csvPath, days);

    return 0;
}

