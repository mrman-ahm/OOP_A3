#ifndef AETHELGARD_H
#define AETHELGARD_H



#include "Classes.h"
#include "InevitableFate.h"
#include "WarHours.h"





class Aethelgard {
private:
    Kingdom* realms[100];
    int countofrealms;
    Kingdom** therebellingfactions;
    int countofrebels;
    TheVoidRift* voidRift;
    int** relations;
    int relationDimension;
    Noble** allthelords; int countoflords;
    Assassin** assassins; int assassinCount;
    FootSoldier** allFootSoldiers; int countoffootsoldiers;
    Knight** allKnights;int countofknights;
    Cavalry** allCavalry;int countofcavalry;
    BatteringRam** allRams;  int countoframs;
    Catapult** allCatapults; int countofcatapults;
    WarShip** allWarShips; int countofships;
    WarriorLord** allWarriorLords; int countofwarriorlords;
    InevitableFate fate;
    int currentDay;
    int threatMax;      
    void doShadyStuff();
    void makePeopleHateEachOther();
    void handleTheFighting();
    void removeTheDead();

    void showWhatsHappening();
    void makeARebellion(int parentIdx);

public:
    Aethelgard();
    ~Aethelgard();
    Kingdom* getRealm(int idx) { return (idx >= 0 && idx < 10) ? realms[idx] : nullptr; }
    void addRealm(Kingdom* k, int idx);
    void addLord(Noble* n);
    void addAssassin(Assassin* a);
    void keepTrackOfFoot(FootSoldier* f);
    void keepTrackOfKnights(Knight* k);
    void keepTrackOfCavalry(Cavalry* c);
    void keepTrackOfRams(BatteringRam* r);
    void keepTrackOfCatapults(Catapult* ca);
    void keepTrackOfShips(WarShip* w);
    void keepTrackOfWarriorLords(WarriorLord* wl);
    void setRelation(int i, int j, int val);
    void setThreatMax(int t);
    void setVoidRift(TheVoidRift* vr);
    bool loadScenario(const char* csvPath);
    void run(const char* csvPath, int days);
};




#endif
