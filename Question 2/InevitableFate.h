#ifndef INEVITABLE_FATE_H
#define INEVITABLE_FATE_H

#include "Classes.h"

struct InevitableFate {
    void updateLordsDaily(Kingdom** realms, int countofrealms, Kingdom** rebels, int countofrebels);

    void resolveSuccession(Kingdom* realm);


    void updateTensionDaily(Kingdom** realms, int countofrealms, Kingdom** rebels, int countofrebels);

    void updatePlotsDaily(Kingdom** realms, int countofrealms, int** relations, Kingdom** rebels, int countofrebels);


    void resolveInternalCoup(Kingdom* realm, LandlessLord* plotter, int** relations, int countofrealms);


    void resolveForeignStrike(Kingdom** realms, int countofrealms, Assassin** assassins, int& aCount, int** relations);

private:
    

    int calculateTotalArmySize(Kingdom* realm);



    Noble* findHighestAmbition(Kingdom* realm);

    bool isBetterEntity(int ambitionA, int idA, const char* nameA, int realmA, int ambitionB, int idB, const char* nameB, int realmB);


};



#endif
