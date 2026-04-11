#ifndef AETHELGARD_H
#define AETHELGARD_H

#include "Classes.h"
#include "InevitableFate.h"
#include "WarHours.h"

// ============================================================
// The Aethelgard Engine
// Sole owner of ALL entity memory. Kingdoms hold unowned refs.
// ============================================================
class Aethelgard {
private:
    // ---- Flat realm array for phase-loop iteration ----
    Kingdom*  realms[10];      // Includes rebel factions once split
    int       realmCount;      // Starts at 10, can grow on faction splits

    // ---- The Void Rift typed pointer (Phase 6) ----
    TheVoidRift* voidRift;     // realms[9] cast to typed ptr

    // ---- The 10x10 Diplomatic Relations Matrix ----
    // relations[i][j] = how realm i feels toward realm j
    // War fires when relations[i][j] < -80 (PDF Section 2.4.1)
    int relations[10][10];

    // ---- Engine-owned entity master lists (Memory Ledger) ----
    // ONLY these arrays may call delete on their elements.
    Noble**    allLords;        int lordCount;
    Assassin** assassins;       int assassinCount;
    // Military units are owned per-realm, swept in phase 7

    // ---- Logic controllers ----
    InevitableFate fate;

    // ---- Simulation state ----
    int currentDay;
    int threatMax;      // From [CONFIG] ThreatMax= in CSV

    // ---- Internal helpers ----

    // Phase 3b/3c: check & fire coups and strikes for the day
    void runSubterfugePhase();

    // Phase 4: every non-self relation drifts by -1 per day
    void runDiplomaticDecay();

    // Phase 5: scan matrix for relations < -80, start war
    void runWarPhase();

    // Phase 7: Engine memory sweep - delete anything isAlive==false
    void runEntityCleanup();

    // Civil War: instantiate a new rebel Kingdom from a parent
    void triggerFactionSplit(int parentIdx);

public:
    Aethelgard();
    ~Aethelgard();

    // Manual setup: called by the CSV parser (or test harness)
    // after all kingdoms, lords, and units are allocated.
    void addRealm(Kingdom* k, int idx);
    void addLord(Noble* n);
    void addAssassin(Assassin* a);
    void setRelation(int i, int j, int val);
    void setThreatMax(int t);
    void setVoidRift(TheVoidRift* vr);

    // --- The Manual Parser (String.h / Repo Strategy) ---
    bool loadScenario(const char* csvPath);

    // Entry point: loads and starts the simulation
    void run(const char* csvPath, int days);
};

#endif
