#include "InevitableFate.h"
#include "string.h"

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
