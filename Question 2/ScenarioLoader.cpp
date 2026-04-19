#include "ScenarioLoader.h"
#include "Aethelgard.h"
#include "custom_string.h"
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
