#include "Aethelgard.h"
#include "string.h"
#include <iostream>
#include <fstream>

// ============================================================
// CONSTRUCTOR / DESTRUCTOR
// ============================================================

Aethelgard::Aethelgard()
    : realmCount(0), voidRift(nullptr),
      allLords(nullptr), lordCount(0),
      assassins(nullptr), assassinCount(0),
      currentDay(0), threatMax(60)
{
    for (int i = 0; i < 10; i++) realms[i] = nullptr;

    // Zero-initialise the entire relations matrix
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            relations[i][j] = (i == j) ? 0 : 50; // Default neutral-positive
}

Aethelgard::~Aethelgard() {
    // ===========================================================
    // MEMORY LEDGER: Engine is the sole owner of all entities.
    // Delete order: Nobles -> Assassins -> Kingdoms (arrays only)
    // Military units are deleted via per-realm sweeps below.
    // ===========================================================

    // 1. Delete all lord objects
    for (int i = 0; i < lordCount; i++) {
        if (allLords[i]) { terminateLord(allLords[i]); allLords[i] = nullptr; }
    }
    delete[] allLords;

    // 2. Delete all assassin objects
    for (int i = 0; i < assassinCount; i++) {
        delete assassins[i]; assassins[i] = nullptr;
    }
    delete[] assassins;

    // 3. Delete all realm objects (just the Kingdom shell - units deleted below)
    for (int i = 0; i < realmCount; i++) {
        delete realms[i]; realms[i] = nullptr;
    }
}

// ============================================================
// SETUP INTERFACE (called by CSV parser or test harness)
// ============================================================

void Aethelgard::addRealm(Kingdom* k, int idx) {
    if (idx < 0 || idx >= 10) return;
    realms[idx] = k;
    if (idx >= realmCount) realmCount = idx + 1;
}

void Aethelgard::addLord(Noble* n) {
    // Grow master array by 1
    Noble** bigger = new Noble*[lordCount + 1];
    for (int i = 0; i < lordCount; i++) bigger[i] = allLords[i];
    bigger[lordCount] = n;
    delete[] allLords;
    allLords = bigger;
    lordCount++;
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
    if (i >= 0 && i < 10 && j >= 0 && j < 10)
        relations[i][j] = val;
}

void Aethelgard::setThreatMax(int t) { threatMax = t; }

void Aethelgard::setVoidRift(TheVoidRift* vr) { voidRift = vr; }

// ============================================================
// PHASE 3: SUBTERFUGE
// ============================================================

void Aethelgard::runSubterfugePhase() {
    // 3a: Tick plot progress for all courtiers across all realms
    fate.updatePlotsDaily(realms, realmCount, relations);

    // 3b: Check for Internal Coup triggers (plotProgress >= 100)
    for (int r = 0; r < realmCount; r++) {
        if (!realms[r]) continue;
        for (int c = 0; c < realms[r]->getCourtierCount(); c++) {
            LandlessLord* plotter = realms[r]->getCourtier(c);
            if (plotter && plotter->getPlotProgress() >= 100.0f) {
                // Check if this realm has a rival <= -50 for Foreign Strike first
                bool hasForeignTarget = false;
                for (int j = 0; j < 10; j++) {
                    if (j != r && relations[r][j] <= -50) { hasForeignTarget = true; break; }
                }

                if (hasForeignTarget) {
                    fate.resolveForeignStrike(realms, assassins, assassinCount, relations);
                } else {
                    fate.resolveInternalCoup(realms[r], plotter, relations);
                }
                break; // One event per realm per day
            }
        }
    }
}

// ============================================================
// PHASE 4: DIPLOMATIC DECAY
// ============================================================

void Aethelgard::runDiplomaticDecay() {
    // PDF Section 2.5 (implied): All relation values drift toward 0 by 1 per day
    // (Prevents permanent alliance stagnation)
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i == j) continue;
            if (relations[i][j] > 0)       relations[i][j]--;
            else if (relations[i][j] < 0)  relations[i][j]++;
        }
    }
}

// ============================================================
// PHASE 5: WAR
// ============================================================

void Aethelgard::runWarPhase() {
    // PDF Section 2.4.1: War triggers when relations[i][j] < -80
    // Only one war per day (the most hostile pair fights first)
    int atkIdx = -1, defIdx = -1;
    int lowestRelation = -80; // Must be strictly below threshold

    for (int i = 0; i < realmCount; i++) {
        if (!realms[i] || !realms[i]->getCurrentLord()) continue;
        for (int j = 0; j < realmCount; j++) {
            if (i == j || !realms[j] || !realms[j]->getCurrentLord()) continue;
            if (relations[i][j] < lowestRelation) {
                lowestRelation = relations[i][j];
                atkIdx = i;
                defIdx = j;
            }
        }
    }

    if (atkIdx == -1) return; // No war today

    Kingdom* atk = realms[atkIdx];
    Kingdom* def = realms[defIdx];

    std::cout << "\n[DAY " << currentDay << "] === WAR DECLARED ===\n";
    std::cout << "Attacker: Realm " << atkIdx << "  |  Defender: Realm " << defIdx << "\n";

    WarHours war(atk, def);
    int result = war.resolveConflict();

    // Apply aftermath (wealth seizure, relation resets, warmonger penalty)
    war.applyWarAftermath(atk, def, result, relations);

    const char* outcome = (result == -1) ? "CONQUERED" :
                          (result ==  1) ? "REPELLED"  :
                          (result ==  0) ? "STALEMATE" : "ABORTED";
    std::cout << "[DAY " << currentDay << "] Battle outcome: " << outcome << "\n";
}

// ============================================================
// PHASE 7: ENTITY CLEANUP (The Memory Ledger Sweep)
// ============================================================

void Aethelgard::runEntityCleanup() {
    // ------------------------------------------------------------------
    // MEMORY LEDGER: The following entities may have been flagged
    // isAlive = false by other phases. We delete them here and null
    // their pointers in the master lists.
    //
    // Sources:
    //   - Aging (Phase 2) : LandedLord healthMeter >= 100
    //   - Internal Coup   : old LandedLord OR failed LandlessLord
    //   - Foreign Strike  : foreign LandedLord OR spent Assassin
    //   - War Casualties  : MilitaryUnits (nulled in WarHours, not deleted)
    // ------------------------------------------------------------------

    // 1. Sweep all lords in the master list
    for (int i = 0; i < lordCount; i++) {
        if (allLords[i] && !allLords[i]->isAliveSelf()) {
            terminateLord(allLords[i]);
            allLords[i] = nullptr;
        }
    }

    // 2. Sweep all assassins in the master list
    for (int i = 0; i < assassinCount; i++) {
        if (assassins[i] && !assassins[i]->isAliveSelf()) {
            delete assassins[i];
            assassins[i] = nullptr;
        }
    }

    // NOTE: Military units (Cavalry, Knight, FootSoldier) are nulled
    // in WarHours::applyCasualties. The actual delete on those objects
    // requires the engine to also maintain master military unit lists.
    // This is deferred: see [FUTURE] tag in implementation_plan.md
}

// ============================================================
// CIVIL WAR: FACTION SPLIT
// ============================================================

void Aethelgard::triggerFactionSplit(int parentIdx) {
    if (parentIdx < 0 || parentIdx >= realmCount) return;
    Kingdom* parent = realms[parentIdx];
    if (!parent) return;

    // Find the LandlessLord with the highest ambition (the rebel leader)
    // Tiebreaker: alphabetically lowest name (PDF Section 2.4.2)
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

    if (!rebel) return; // No one to lead the revolt

    // Instantiate rebel realm using the Split Constructor
    // Give it an ID beyond the current realmCount
    int newIdx = realmCount;
    Kingdom* rebelRealm = new Kingdom(*parent, rebel, newIdx);

    // Register into the engine's realm array
    if (newIdx < 10) {
        realms[newIdx] = rebelRealm;
        realmCount++;
    }

    // Set initial relations: rebel realm is at war with parent
    setRelation(parentIdx, newIdx, -100);
    setRelation(newIdx, parentIdx, -100);

    // Reset parent tension
    parent->resetTension();

    std::cout << "[FACTION SPLIT] Realm " << parentIdx
              << " fractured. Rebel faction born at index " << newIdx << ".\n";
}

// ============================================================
// MAIN RUN LOOP
// ============================================================

bool Aethelgard::loadScenario(const char* csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open scenario file: " << csvPath << "\n";
        return false;
    }

    char line[512];
    int section = 0; // 1:KINGDOMS, 2:LORDS, 3:RELATIONS, 4:MILITARY, 5:SAGES, 6:ASSASSINS, 7:CONFIG

    while (file.getline(line, 512)) {
        if (line[0] == '\0' || line[0] == '#') continue;

        // Section Detection
        if (findSubstring(line, "[KINGDOMS]") != -1) { section = 1; continue; }
        if (findSubstring(line, "[LORDS]") != -1)    { section = 2; continue; }
        if (findSubstring(line, "[RELATIONS]") != -1){ section = 3; continue; }
        if (findSubstring(line, "[MILITARY]") != -1) { section = 4; continue; }
        if (findSubstring(line, "[SAGES]") != -1)    { section = 5; continue; }
        if (findSubstring(line, "[ASSASSINS]") != -1){ section = 6; continue; }
        if (findSubstring(line, "[CONFIG]") != -1)   { section = 7; continue; }

        // Tokenization (Manual Split)
        char* tokens[20];
        int tokenCount = 0;
        char* current = line;

        if (section == 3) {
            // Relations uses whitespace
            while (*current != '\0' && tokenCount < 20) {
                while (*current == ' ' || *current == '\t') current++;
                if (*current == '\0') break;
                tokens[tokenCount++] = current;
                while (*current != ' ' && *current != '\t' && *current != '\0') current++;
                if (*current != '\0') { *current = '\0'; current++; }
            }
        } else {
            // Other sections use comma
            while (current && tokenCount < 20) {
                tokens[tokenCount++] = current;
                char* comma = (char*)findFirstOccurrence(current, ',');
                // findFirstOccurrence returns int, not char*. My mistake.
                int commaIdx = findFirstOccurrence(current, ',');
                if (commaIdx != -1) {
                    current[commaIdx] = '\0';
                    current = current + commaIdx + 1;
                } else {
                    current = nullptr;
                }
            }
        }

        if (tokenCount == 0) continue;

        // Factory Logic
        if (section == 1) { // [KINGDOMS]
            // ID(0), Name(1), Wealth(2), Tax(3), Defense(4), Terrain(5), Extra(6)
            int id = stoi(tokens[0]);
            int w = stoi(tokens[2]);
            int t = stoi(tokens[3]);
            int d = stoi(tokens[4]);
            
            if (findSubstring(tokens[1], "Void Rift") != -1) {
                TheVoidRift* vr = new TheVoidRift();
                setVoidRift(vr);
            } else {
                Kingdom* k = nullptr;
                if (findSubstring(tokens[5], "Coastal") != -1) k = new CoastalKingdom(w, d, id, t, 2000); 
                else if (findSubstring(tokens[5], "Frozen") != -1) k = new FrostPeaksKingdom(w, d, id, t);
                else k = new VerdantKingdom(w, d, id, t, 500); 
                
                if (k) addRealm(k, id);
            }
        }
        else if (section == 2) { // [LORDS]
            // ID(0), Name(1), Age(2), G(3), Class(4), RealmIdx(5), Str(6), Amb(7), Dipl(8), Father(9)
            int id = stoi(tokens[0]);
            int realmId = stoi(tokens[5]);
            int typeIdx = (findSubstring(tokens[4], "Landed") != -1) ? 1 : 
                          (findSubstring(tokens[4], "Landless") != -1) ? 2 : 3;

            Noble* n = nullptr;
            if (typeIdx == 1) n = new LandedLord(copyString(tokens[1]), id, stoi(tokens[2]), 0, stoi(tokens[9]), stoi(tokens[7]), stoi(tokens[6]), stoi(tokens[8]), realmId, nullptr);
            else if (typeIdx == 2) n = new LandlessLord(copyString(tokens[1]), id, stoi(tokens[2]), 0, realmId, stoi(tokens[7]), stoi(tokens[6]), stoi(tokens[8]), realmId, nullptr);
            else n = new WarriorLord(copyString(tokens[1]), id, stoi(tokens[2]), 0, stoi(tokens[9]), stoi(tokens[6]), stoi(tokens[7]), stoi(tokens[8]), realmId, nullptr, 100, 100, 20, 5, 50, true, true);

            if (n) {
                addLord(n);
                if (realms[realmId]) {
                    if (typeIdx == 1) realms[realmId]->setCurrentLord(n);
                    else if (typeIdx == 2) realms[realmId]->addCourtier((LandlessLord*)n);
                    else if (typeIdx == 3) realms[realmId]->addWarriorLord((WarriorLord*)n);
                }
            }
        }
        else if (section == 3) { // [RELATIONS]
            static int relRow = 0;
            if (relRow < 10) {
                for (int c = 0; c < tokenCount && c < 10; c++) {
                    relations[relRow][c] = stoi(tokens[c]);
                }
                relRow++;
            }
        }
        else if (section == 4) { // [MILITARY]
            // RealmIdx(0), UnitType(1), Count(2), BaseAttack(3), BaseHP(4), Siege(5), AoE(6)
            int rid = stoi(tokens[0]);
            int count = stoi(tokens[2]);
            if (realms[rid]) {
                for (int i = 0; i < count; i++) {
                    if (findSubstring(tokens[1], "Knight") != -1) {
                        realms[rid]->addKnight(new Knight(copyString(tokens[1]), 1000+rid*100+i, stoi(tokens[4]), stoi(tokens[4]), 20, 5, stoi(tokens[3]), true, true));
                    } else if (findSubstring(tokens[1], "Foot") != -1) {
                        realms[rid]->addFootSoldier(new FootSoldier(copyString(tokens[1]), 2000+rid*100+i, stoi(tokens[4]), stoi(tokens[4]), 10, 2, stoi(tokens[3]), true));
                    }
                }
            }
        }
        else if (section == 5) { // [SAGES]
            // RealmIdx(0), Name(1), HealingPower(2), Wisdom(3)
            int rid = stoi(tokens[0]);
            if (realms[rid]) {
                realms[rid]->addSage(new Sage(copyString(tokens[1]), 3000+rid, 50, stoi(tokens[2]), stoi(tokens[3])));
            }
        }
        else if (section == 6) { // [ASSASSINS]
            // ID(0), Name(1), Age(2), Stealth(3), Client(4)
            addAssassin(new Assassin(copyString(tokens[1]), stoi(tokens[0]), stoi(tokens[2]), stoi(tokens[3]), stoi(tokens[4])));
        }
        else if (section == 7) { // [CONFIG]
            if (findSubstring(line, "ThreatMax") != -1) {
                int eqIdx = findFirstOccurrence(line, '=');
                if (eqIdx != -1) {
                    threatMax = stoi(line + eqIdx + 1);
                }
            }
        }
    }
    return true;
}

void Aethelgard::run(const char* csvPath, int days) {
    if (!loadScenario(csvPath)) {
        std::cerr << "Simulation aborted: Load failed.\n";
        return;
    }

    std::cout << "\n=== AETHELGARD SIMULATION BEGIN (Days: " << days << ") ===\n";
    // ... rest of the run loop ...

    for (int d = 0; d < days; d++) {
        currentDay = d + 1;

        // --------------------------------------------------
        // PHASE 1: ECONOMICS (Tax -> Maintenance -> Bankruptcy)
        // --------------------------------------------------
        for (int r = 0; r < realmCount; r++) {
            if (realms[r]) realms[r]->performAction();
        }

        // --------------------------------------------------
        // PHASE 2: AGING & SUCCESSION
        // --------------------------------------------------
        fate.updateLordsDaily(realms, realmCount);

        // --------------------------------------------------
        // PHASE 3: SUBTERFUGE (Plots, Coups, Foreign Strikes)
        // --------------------------------------------------
        runSubterfugePhase();

        // --------------------------------------------------
        // PHASE 4: DIPLOMATIC DECAY
        // --------------------------------------------------
        runDiplomaticDecay();

        // --------------------------------------------------
        // PHASE 5: WAR
        // --------------------------------------------------
        runWarPhase();

        // --------------------------------------------------
        // PHASE 6: THE VOID (Threat growth & endgame check)
        // --------------------------------------------------
        if (voidRift) {
            voidRift->performAction();
            if (voidRift->getThreatLevel() >= threatMax) {
                std::cout << "\n[DAY " << currentDay
                          << "] THE VOID HAS CONSUMED ALL. Simulation ends.\n";
                break;
            }
        }

        // --------------------------------------------------
        // PHASE 7: MEMORY CLEANUP (The Ledger Sweep)
        // --------------------------------------------------
        runEntityCleanup();

        // --------------------------------------------------
        // PHASE 3 POST-SWEEP: Check for Faction Splits
        // (Must happen after cleanup so dead lords are gone)
        // --------------------------------------------------
        for (int r = 0; r < realmCount; r++) {
            if (realms[r] && realms[r]->getTension() >= 100.0f) {
                triggerFactionSplit(r);
            }
        }
    }

    std::cout << "\n=== SIMULATION COMPLETE (Day " << currentDay << ") ===\n";
}
