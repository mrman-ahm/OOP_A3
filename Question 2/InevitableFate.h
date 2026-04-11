#ifndef INEVITABLE_FATE_H
#define INEVITABLE_FATE_H

#include "Classes.h"

struct InevitableFate {
    // The Kingdom Crawler: Increments age and health decay for all realm members
    void updateLordsDaily(Kingdom** realms, int realmCount);

    // Succession Logic: Promotes the most ambitious local courtier to the throne
    void resolveSuccession(Kingdom* realm);

    // Tension Logic: Calculates daily stability shifts based on realm state
    void updateTensionDaily(Kingdom** realms, int realmCount);

    // Phase 3b: Plot Logic: Accumulates assassination progress for courtiers
    void updatePlotsDaily(Kingdom** realms, int realmCount, int relations[10][10]);

    // Phase 3b: Internal Coup: Executes a plotter's strike against their own lord
    void resolveInternalCoup(Kingdom* realm, LandlessLord* plotter, int relations[10][10]);

    // Phase 3c: Foreign Strike: Executes hired assassins against rival realm lords
    void resolveForeignStrike(Kingdom** realms, Assassin** assassins, int& aCount, int relations[10][10]);

private:
    // Manual summation of all unit references across all categories
    int calculateTotalArmySize(Kingdom* realm);

    // Internal helper to find the best candidate in a specific realm
    // Returns a pointer to either a LandlessLord or a WarriorLord (via Noble base cast)
    Noble* findHighestAmbition(Kingdom* realm);
};

#endif
