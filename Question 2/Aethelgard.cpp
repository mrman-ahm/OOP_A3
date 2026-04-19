#include "Aethelgard.h"
#include "custom_string.h"
#include "ScenarioLoader.h"
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
