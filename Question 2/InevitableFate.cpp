#include "InevitableFate.h"
#include "string.h"

void InevitableFate::updateLordsDaily(Kingdom** realms, int realmCount) {
    if (realms == nullptr) return;

    for (int r = 0; r < realmCount; r++) {
        Kingdom* realm = realms[r];
        if (realm == nullptr) continue;

        // PDF Table I: Only LandedLords and LandlessLords age daily.
        // Age the local Courtiers (LandlessLords)
        for (int i = 0; i < realm->courtierCount; i++) { if (realm->courtiers[i]) realm->courtiers[i]->age++; }

        // Age the Ruler (LandedLord) and check Geriatric Decay
        if (realm->currentLord != nullptr) {
            realm->currentLord->age++;
            // The Weight of the Crown: Geriatric Decay (70+)
            if (realm->currentLord->age >= 70) {
                realm->currentLord->healthMeter += 30.0f;
            }
            // Mortality Trigger: HealthMeter >= 100.0
            if (realm->currentLord->healthMeter >= 100.0f) {
                realm->currentLord->isAlive = false; // Flag for Engine cleanup
                realm->currentLord = nullptr;
                resolveSuccession(realm);
            }
        }
    }
}

Noble* InevitableFate::findHighestAmbition(Kingdom* realm) {
    if (realm == nullptr) return nullptr;
    
    Noble* currentBest = nullptr;
    int maxAmbition = -1;

    // --- PHASE 1: FIND MAX AMBITION (LOCAL ARRAYS) ---
    for (int j = 0; j < realm->warriorLordCount; j++) {
        if (realm->warriorLords[j] && realm->warriorLords[j]->ambitionStat > maxAmbition) {
            maxAmbition = realm->warriorLords[j]->ambitionStat;
        }
    }
    for (int i = 0; i < realm->courtierCount; i++) {
        if (realm->courtiers[i] && realm->courtiers[i]->ambitionStat > maxAmbition) {
            maxAmbition = realm->courtiers[i]->ambitionStat;
        }
    }

    // --- PHASE 2: RESOLVE WINNER (COURTIER SCAN) ---
    // Tiebreaker order: 1. Lower Entity ID, 2. Alphabetically lower name
    for (int i = 0; i < realm->courtierCount; i++) {
        if (realm->courtiers[i] && realm->courtiers[i]->ambitionStat == maxAmbition) {
            if (currentBest == nullptr) {
                currentBest = realm->courtiers[i];
            } else if (realm->courtiers[i]->id < currentBest->id) {
                currentBest = realm->courtiers[i];
            } else if (realm->courtiers[i]->id == currentBest->id && isSmallerThan(realm->courtiers[i]->name, currentBest->name)) {
                currentBest = realm->courtiers[i];
            }
        }
    }

    // --- PHASE 3: RESOLVE WINNER (WARRIOR SCAN) ---
    for (int j = 0; j < realm->warriorLordCount; j++) {
        if (realm->warriorLords[j] && realm->warriorLords[j]->ambitionStat == maxAmbition) {
            if (currentBest == nullptr) {
                currentBest = realm->warriorLords[j];
            } else if (realm->warriorLords[j]->Noble::id < currentBest->id) {
                currentBest = realm->warriorLords[j];
            } else if (realm->warriorLords[j]->Noble::id == currentBest->id && isSmallerThan(realm->warriorLords[j]->Noble::name, currentBest->name)) {
                currentBest = realm->warriorLords[j];
            }
        }
    }

    return currentBest;
}

void InevitableFate::resolveSuccession(Kingdom* realm) {
    if (realm == nullptr) return;

    // 1. Find the best successor purely from local court pools
    Noble* winner = findHighestAmbition(realm);

    if (winner != nullptr) {
        // 2. Kill the previous lord
        // Engine owns memory — we only null the pointer and flag for Engine cleanup.
        if (realm->currentLord != nullptr) {
            realm->currentLord->isAlive = false; // Engine will call terminateLord on its next sweep
            realm->currentLord = nullptr;
        }

        // 3. The Promotion: Physically point currentLord to the winner
        realm->currentLord = winner;

        // 4. THE POP: Remove the new ruler from their old local subject-arrays
        bool foundAndRemoved = false;

        // Scanning local warriorLords FIRST
        for (int j = 0; j < realm->warriorLordCount && !foundAndRemoved; j++) {
            if (realm->warriorLords[j] == (WarriorLord*)winner) {
                // Shifting elements to preserve relative order
                for (int k = j; k < realm->warriorLordCount - 1; k++) {
                    realm->warriorLords[k] = realm->warriorLords[k + 1];
                }
                realm->warriorLordCount--;
                foundAndRemoved = true;
            }
        }

        // Scanning local courtiers SECOND
        for (int i = 0; i < realm->courtierCount && !foundAndRemoved; i++) {
            if (realm->courtiers[i] == (LandlessLord*)winner) {
                // Shifting elements to preserve relative order
                for (int k = i; k < realm->courtierCount - 1; k++) {
                    realm->courtiers[k] = realm->courtiers[k + 1];
                }
                realm->courtierCount--;
                foundAndRemoved = true;
            }
        }
    }
}

int InevitableFate::calculateTotalArmySize(Kingdom* realm) {
    if (realm == nullptr) return 0;
    
    // Inefficient manual summation of every single military reference array count
    int totalUnits = realm->footSoldierCount + realm->knightCount + realm->cavalryCount + realm->ramCount + realm->catapultCount + realm->warShipCount + realm->warriorLordCount;
    
    return totalUnits;
}

void InevitableFate::updateTensionDaily(Kingdom** realms, int realmCount) {
    for (int r = 0; r < realmCount; r++) {
        if (realms[r] == nullptr) continue;
        
        float netTensionChange = 0.0f;
        
        // 1. Economic Insolvency: Wealth < 0 (+5.0) (SHUFFLED PHASE)
        if (realms[r]->wealth < 0) netTensionChange += 5.0f;

        // 2. Political Instability: The absence of a currentLord (+30.0)
        if (realms[r]->currentLord == nullptr) netTensionChange += 30.0f;
        else if (realms[r]->currentLord->strategyStat > 70) netTensionChange -= 3.0f;

        // 4. Military Suppression: Massive Army > 200 units (-1.0)
        if (calculateTotalArmySize(realms[r]) > 200) netTensionChange -= 1.0f;
        
        // 5. Subversive Courtiers: LandlessLord Ambition > 50 (+Ambition / 10.0)
        for (int c = 0; c < realms[r]->courtierCount; c++) { if (realms[r]->courtiers[c] != nullptr && realms[r]->courtiers[c]->ambitionStat > 50) netTensionChange += (realms[r]->courtiers[c]->ambitionStat / 10.0f); }
        
        // Apply Delta
        realms[r]->civilWarTension += netTensionChange;
        if (realms[r]->civilWarTension < 0.0f) realms[r]->civilWarTension = 0.0f;

    }
}

void InevitableFate::updatePlotsDaily(Kingdom** realms, int realmCount, int relations[10][10]) {
    if (realms == nullptr) return;

    for (int r = 0; r < realmCount; r++) {
        Kingdom* realm = realms[r];
        if (realm == nullptr) continue;

        // Section 2.4.3: Landless courtiers (Ambition > 60) track plot progress
        for (int i = 0; i < realm->courtierCount; i++) {
            LandlessLord* plotter = realm->courtiers[i];
            if (!plotter || plotter->ambitionStat <= 60) continue;

            // 1. Base Trajectory: (Ambition - 60) / 5.0
            float tick = (plotter->ambitionStat - 60) / 5.0f;

            // 2. Compound Bonus: +2.0 daily if realm has ANY negative relation
            bool hasEnemies = false;
            for (int rival = 0; rival < 10; rival++) {
                if (relations[r][rival] < 0) {
                    hasEnemies = true;
                    break;
                }
            }
            if (hasEnemies) tick += 2.0f;

            // 3. Native Stunting: -(Strategy / 25.0)
            if (realm->currentLord) {
                tick -= (realm->currentLord->strategyStat / 25.0f);
            }

            // 4. Garrisoned Stunting: Defense > 50 subtracts 1.0
            if (realm->defenseStat > 50) {
                tick -= 1.0f;
            }

            // 5. Floor at zero
            if (tick < 0.0f) tick = 0.0f;

            // Apply and check for Execution (Phase 3b)
            plotter->assassinationPlotProgress += tick;
            
            // If the apex (100.0) is reached, the Engine will call resolveInternalCoup or ForeignStrike
            // (Handled by the daily phase loop in Aethelgard.cpp)
        }
    }
}

void InevitableFate::resolveInternalCoup(Kingdom* realm, LandlessLord* plotter, int relations[10][10]) {
    if (!realm || !plotter || !realm->currentLord) return;

    // Strike Calculation (Section 2.4.3)
    // Assassin Power = Plotter.AmbitionStat + (Realm.Wealth / 100.0)
    float power = plotter->ambitionStat + (realm->wealth / 100.0f);

    // Target Defense = Ruler.StrategyStat + (Realm.DefenseStat / 10.0) + AgeModifier
    // Ruler age < 40 (+5), Ruler age > 40 (-5)
    float ageMod = (realm->currentLord->age < 40) ? 5.0f : -5.0f;
    float defense = realm->currentLord->strategyStat + (realm->defenseStat / 10.0f) + ageMod;

    if (power > defense) {
        // SUCCESS: Terminate Ruler
        realm->currentLord->isAlive = false; // Engine sweep cleans ID
        realm->currentLord = nullptr;
        
        // Instant Succession
        resolveSuccession(realm);
    } else {
        // FAILURE: Terminate Plotter
        plotter->isAlive = false; // Engine sweep cleans ID
        
        // Diplomatic Penalty: -30 with ALL other realms
        for (int j = 0; j < 10; j++) {
            if (j != realm->realmID) {
                relations[realm->realmID][j] -= 30;
                relations[j][realm->realmID] -= 30; // Symmetric decay assumed for balance
            }
        }
    }

    // Always zero out the plot on strike
    plotter->assassinationPlotProgress = 0.0f;
}

void InevitableFate::resolveForeignStrike(Kingdom** realms, Assassin** assassins, int& aCount, int relations[10][10]) {
    if (!realms || !assassins) return;

    // Iterate through all active assassins tracked by the engine
    for (int i = 0; i < aCount; i++) {
        Assassin* agent = assassins[i];
        if (!agent || !agent->isAlive) continue;

        int clientIdx = agent->clientIdx;
        Kingdom* clientRealm = realms[clientIdx];
        if (!clientRealm) continue;

        // 1. Check for valid strike condition: plot apex 100.0 exists in the client realm
        LandlessLord* plotter = nullptr;
        for (int p = 0; p < clientRealm->courtierCount; p++) {
            if (clientRealm->courtiers[p] && clientRealm->courtiers[p]->assassinationPlotProgress >= 100.0f) {
                plotter = clientRealm->courtiers[p];
                break;
            }
        }
        if (!plotter) continue;

        // 2. Identify the Target: Lowest Relation <= -50 (Section 2.4.4)
        int targetIdx = -1;
        int minRelation = 0;
        for (int r = 0; r < 10; r++) {
            if (r != clientIdx && relations[clientIdx][r] <= -50) {
                if (targetIdx == -1 || relations[clientIdx][r] < minRelation) {
                    minRelation = relations[clientIdx][r];
                    targetIdx = r;
                }
            }
        }
        
        // If NO rival is hated enough, the strike doesn't fire (plot remains at 100 for internal coup)
        if (targetIdx == -1) continue;

        Kingdom* targetRealm = realms[targetIdx];
        if (!targetRealm || !targetRealm->currentLord) continue;

        // 3. The contest: StealthStat > StrategyStat
        if (agent->stealthStat > targetRealm->currentLord->strategyStat) {
            // SUCCESS: Foreign Ruler Terminated
            targetRealm->currentLord->isAlive = false;
            targetRealm->currentLord = nullptr;
            resolveSuccession(targetRealm);
        } else {
            // FAILURE: Diplomatic Blowback (Relations[Target][Client] -= 20)
            relations[targetIdx][clientIdx] -= 20;
        }

        // ALWAYS: Spent Assassin is removed (Section 2.4.4)
        agent->isAlive = false;
        
        // ALWAYS: Zero out the trigger plot
        plotter->assassinationPlotProgress = 0.0f;
        
        // Stop after firing one strike per loop cycle (simplification)
        break; 
    }
}
