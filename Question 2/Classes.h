#ifndef CLASSES_HEADER
#define CLASSES_HEADER

#include <iostream>


class Entity {
protected:
    char* name; 
    int id;
    friend struct InevitableFate;
    friend struct WarHours;
public:
    Entity(const char* n, int i) : id(i) {
        int len = 0;
        while(n[len] != '\0') len++;
        name = new char[len + 1];
        for(int j = 0; j <= len; j++) name[j] = n[j];
        name[len] = '\0';
    }
    ~Entity() { delete[] name; }
    const char* getName() const { return name; }
    int getID() const { return id; }
};

class Person : public Entity {
protected:
    bool isAlive;
    int age;
    friend struct InevitableFate;
public:
    Person(const char* n, int i, int a) : Entity(n, i), isAlive(true), age(a) {} 
    void setAlive(bool alive) { isAlive = alive; }
    bool isAliveSelf() const { return isAlive; }
};

class Asset : public Entity {
protected:
    int maxHp;
    int hp;
    int maintenanceCost;
    int armor;
    friend struct WarHours;
public:
    Asset(const char* n, int i, int h, int mH, int ar, int cost) : Entity(n, i), maxHp(mH), hp(h), maintenanceCost(cost), armor(ar) {}
    void takeDmg(int damage);
    void takedefaultDmg(int damage);
    int getMaintenanceCost() const { return maintenanceCost; }
};



class MilitaryUnit : public Asset {
protected:
    int attackPower;
    friend struct WarHours;
public:
    MilitaryUnit(const char* n, int i, int h, int mH, int ar, int cost, int ap) : Asset(n, i, h, mH, ar, cost), attackPower(ap) {}
};

class MeleeUnit : public MilitaryUnit {
protected:
    bool servesFrontline;
public:
    MeleeUnit(const char* n, int i, int h, int mH, int ar, int cost, int ap, bool frontline) : MilitaryUnit(n, i, h, mH, ar, cost, ap), servesFrontline(frontline) {}
};

class FootSoldier : public MeleeUnit {
public:
    FootSoldier(const char* n, int i, int h, int mH, int ar, int cost, int ap, bool frontline) : MeleeUnit(n, i, h, mH, ar, cost, ap, frontline) {}
};

class Knight : public MeleeUnit {
protected:
    bool shieldBlock;
public:
    Knight(const char* n, int i, int h, int mH, int ar, int cost, int ap, bool frontline, bool shield) : MeleeUnit(n, i, h, mH, ar, cost, ap, frontline), shieldBlock(shield) {}
    bool isShieldActive() const { return shieldBlock; }
};

class MountedUnit : public MilitaryUnit {
protected:
    float terrainPenalty;
    float chargeMultiplier;
    friend struct WarHours;
public:
    MountedUnit(const char* n, int i, int h, int mH, int ar, int cost, int ap, float cm, float tp) : MilitaryUnit(n, i, h, mH, ar, cost, ap), terrainPenalty(tp), chargeMultiplier(cm) {}
};

class Cavalry : public MountedUnit {
public:
    Cavalry(const char* n, int i, int h, int mH, int ar, int cost, int ap, float cm, float tp) : MountedUnit(n, i, h, mH, ar, cost, ap, cm, tp) {}
};

class Sage : public Person {
protected:
    int wisdom;
    int healingPower;
    friend struct WarHours;
public:
    Sage(const char* n, int i, int a, int hpw, int wis) : Person(n, i, a), wisdom(wis), healingPower(hpw) {}
};

class Assassin : public Person {
protected:
    int clientIdx;
    int stealthStat;
    friend struct InevitableFate;
    friend class Aethelgard;
public:
    Assassin(const char* n, int i, int a, int stealth, int client) : Person(n, i, a), clientIdx(client), stealthStat(stealth) {}
};


class SiegeEngine : public Asset {
protected:
    int speed;
    int siegeDamage;
    friend struct WarHours;
public:
    SiegeEngine(const char* n, int i, int h, int mH, int ar, int cost, int sdmg, int spd) : Asset(n, i, h, mH, ar, cost), speed(spd), siegeDamage(sdmg) {}
};

class BatteringRam : public SiegeEngine {
public:
    BatteringRam(const char* n, int i, int h, int mH, int ar, int cost, int sdmg, int spd) : SiegeEngine(n, i, h, mH, ar, cost, sdmg, spd) {}
};

class Catapult : public SiegeEngine {
protected:
    int aoeTargets;
    int aeDamage;
public:
    Catapult(const char* n, int i, int h, int mH, int ar, int cost, int sdmg, int spd, int aoeD, int aoeT) : SiegeEngine(n, i, h, mH, ar, cost, sdmg, spd), aoeTargets(aoeT), aeDamage(aoeD) {}
};

class Transport : public Asset {
protected:
    int knotsSpeed;
    int troopCapacity;
public:
    Transport(const char* n, int i, int h, int mH, int ar, int cost, int capacity, int knots) : Asset(n, i, h, mH, ar, cost), knotsSpeed(knots), troopCapacity(capacity) {}
};

class Noble : public Person {
protected:
    int childCount;
    int fatherId;
    int typeCode; 
    int strategyStat;
    int ambitionStat;
    float healthMeter;
    int diplomacyStat;
    int realmIdx;
    int* childrenIds; 
    friend struct WarHours;
    friend struct InevitableFate;
    friend void terminateLord(Noble* n);
public:
    Noble(const char* n, int tc, int i, int a, int cCount, int fId, int strat, int amb, int diplo, int rIdx, int* cIds) : Person(n, i, a), childCount(cCount), fatherId(fId), typeCode(tc), strategyStat(strat), ambitionStat(amb), healthMeter(0.0f), diplomacyStat(diplo), realmIdx(rIdx) {
        if (cCount > 0) {
            childrenIds = new int[cCount];
            for(int j = 0; j < cCount; j++) childrenIds[j] = cIds[j];
        }
        else {
            childrenIds = nullptr;
        }
    }
    ~Noble() { delete[] childrenIds; }
    int getAmbitionStat() const { return ambitionStat; }
    int getTypeCode() const { return typeCode; }
};

class LandedLord : public Noble {
public:
    LandedLord(const char* n, int i, int a, int cCount, int fId, int strat, int amb, int diplo, int rIdx, int* cIds) : Noble(n, 1, i, a, cCount, fId, strat, amb, diplo, rIdx, cIds) {}
};

class LandlessLord : public Noble {
protected:
    float assassinationPlotProgress;
    friend struct InevitableFate;
public:
    LandlessLord(const char* n, int i, int a, int cCount, int fId, int strat, int amb, int diplo, int rIdx, int* cIds) : Noble(n, 2, i, a, cCount, fId, strat, amb, diplo, rIdx, cIds), assassinationPlotProgress(0.0f) {}
    float getPlotProgress() const { return assassinationPlotProgress; }
};



class WarriorLord : public Noble, public Knight {
public:
    WarriorLord(const char* n, int id, int a, int cCount, int fId, int strat, int amb, int diplo, int rIdx, int* cIds, int h, int mH, int ar, int cost, int ap, bool frontline, bool shield) : Noble(n, 3, id, a, cCount, fId, strat, amb, diplo, rIdx, cIds), Knight(n, id, h, mH, ar, cost, ap, frontline, shield) {}

};

class WarShip : public Transport, public SiegeEngine {
public:
    WarShip(int id, const char* n, int h, int mH, int ar, int cost, int capacity, int knots, int sdmg, int spd) : Transport(n, id, h, mH, ar, cost, capacity, knots), SiegeEngine(n, id, h, mH, ar, cost, sdmg, spd) {}

    void takeHullDamage(int dmg) {
        int damagetaken= dmg - Transport::armor; if(damagetaken<0) damagetaken=0;
        Transport::hp -= damagetaken;
    }

    void takeWeaponDamage(int dmg) {
        int damagetaken= dmg - SiegeEngine::armor; if(damagetaken<0) damagetaken=0;
        SiegeEngine::hp -= damagetaken;
    }
};




class Kingdom {
protected:


    float civilWarTension;
    int realmID;


    int countoffootsoldiers;
    FootSoldier** footSoldiers;

    int countofcatapults;
    Catapult** catapults;


    int countofwarriorlords;
    WarriorLord** warriorLords; 

    int wealth;
    Noble* currentLord;

    int countofknights;
    Knight** knights;


    int countofsages;
    Sage** sages;

    int baseTaxIncome;
    int countofcavalry;
    Cavalry** cavalry;
     int defenseStat;


    int countoframs;
    BatteringRam** rams;

    int countofheirs;
    int* heirIds;

    int countofcourtiers;
    LandlessLord** courtiers;

    int countofships;
    WarShip** warShips;

    int terrainCode; 

    friend struct WarHours;
    friend struct InevitableFate;
    friend class Aethelgard;



public:
    Kingdom(int w, int d, int id, int tax) : civilWarTension(0.0f), realmID(id), countoffootsoldiers(0), footSoldiers(nullptr), countofcatapults(0), catapults(nullptr), countofwarriorlords(0), warriorLords(nullptr), wealth(w), currentLord(nullptr), countofknights(0), knights(nullptr), countofsages(0), sages(nullptr), baseTaxIncome(tax), countofcavalry(0), cavalry(nullptr), defenseStat(d), countoframs(0), rams(nullptr), countofheirs(0), heirIds(nullptr), countofcourtiers(0), courtiers(nullptr), countofships(0), warShips(nullptr), terrainCode(0) {}

    ~Kingdom(); 

    Kingdom(Kingdom& parent, LandlessLord* rebelLeader, int newID) : civilWarTension(0.0f), realmID(newID), terrainCode(parent.terrainCode) {
        this->wealth = parent.wealth / 2; parent.wealth -= this->wealth;
        this->baseTaxIncome = parent.baseTaxIncome / 2; parent.baseTaxIncome -= this->baseTaxIncome;
        this->defenseStat = parent.defenseStat / 2; parent.defenseStat -= this->defenseStat;

        this->currentLord = (Noble*)rebelLeader;


        if (this->currentLord) this->currentLord->setAlive(true);

        this->countoffootsoldiers = parent.countoffootsoldiers / 2;
        this->footSoldiers = new FootSoldier*[2000]; 
        for (int i = 0; i < this->countoffootsoldiers; i++) {
            this->footSoldiers[i] = parent.footSoldiers[parent.countoffootsoldiers - 1 - i];
            parent.footSoldiers[parent.countoffootsoldiers - 1 - i] = nullptr;
        }
        parent.countoffootsoldiers -= this->countoffootsoldiers;

        this->countofknights = parent.countofknights / 2;
        this->knights = new Knight*[1000];

        for (int i = 0; i < this->countofknights; i++) {
            this->knights[i] = parent.knights[parent.countofknights - 1 - i];
            parent.knights[parent.countofknights - 1 - i] = nullptr;
        }
        parent.countofknights -= this->countofknights;

        this->countofcavalry = parent.countofcavalry / 2;
        this->cavalry = new Cavalry*[500];

        for (int i = 0; i < this->countofcavalry; i++) {
            this->cavalry[i] = parent.cavalry[parent.countofcavalry - 1 - i];
            parent.cavalry[parent.countofcavalry - 1 - i] = nullptr;
        }
        parent.countofcavalry -= this->countofcavalry;



        this->countofships = parent.countofships / 2;
        this->warShips = new WarShip*[200];
        for (int i = 0; i < this->countofships; i++) {
            this->warShips[i] = parent.warShips[parent.countofships - 1 - i];
            parent.warShips[parent.countofships - 1 - i] = nullptr;
        }
        parent.countofships -= this->countofships;


        this->countofcatapults = parent.countofcatapults / 2;
        this->catapults = new Catapult*[100];
        for (int i = 0; i < this->countofcatapults; i++) {
            this->catapults[i] = parent.catapults[parent.countofcatapults - 1 - i];
            parent.catapults[parent.countofcatapults - 1 - i] = nullptr;
        }
        parent.countofcatapults -= this->countofcatapults;


        this->countoframs = parent.countoframs / 2;
        this->rams = new BatteringRam*[100];
        for (int i = 0; i < this->countoframs; i++) {
            this->rams[i] = parent.rams[parent.countoframs - 1 - i];
            parent.rams[parent.countoframs - 1 - i] = nullptr;
        }
        parent.countoframs -= this->countoframs;




        this->countofwarriorlords = parent.countofwarriorlords / 2;
        this->warriorLords = new WarriorLord*[20];
        for (int i = 0; i < this->countofwarriorlords; i++) {
            this->warriorLords[i] = parent.warriorLords[parent.countofwarriorlords - 1 - i];
            parent.warriorLords[parent.countofwarriorlords - 1 - i] = nullptr;
        }
        parent.countofwarriorlords -= this->countofwarriorlords;

        this->countofsages = 0; this->sages = nullptr;
        this->countofcourtiers = 0; this->courtiers = nullptr;
        this->countofheirs = 0; this->heirIds = nullptr;




    }

    int calculateMaintenance() {
        int total = 0;


        for (int i = 0; i < countoffootsoldiers; i++) 
            if (footSoldiers[i]) total += footSoldiers[i]->getMaintenanceCost();
        for (int i = 0; i < countofknights; i++) 
            if (knights[i]) total += knights[i]->getMaintenanceCost();
        for (int i = 0; i < countofcavalry; i++) 
            if (cavalry[i]) total += cavalry[i]->getMaintenanceCost();
        for (int i = 0; i < countoframs; i++) 
            if (rams[i]) total += rams[i]->getMaintenanceCost();
        for (int i = 0; i < countofcatapults; i++) 
            if (catapults[i]) total += catapults[i]->getMaintenanceCost();
        for (int i = 0; i < countofships; i++) 
            if (warShips[i]) total += warShips[i]->Transport::getMaintenanceCost();


        return total;
    }

    void updateStandardDuties() {
        wealth += baseTaxIncome;
        
        wealth -= calculateMaintenance();

        if (wealth < 0) {
            wealth = 0;
            int loss = countoffootsoldiers * 0.05f; 
            if (loss > 0) {
                for (int i = countoffootsoldiers - loss; i < countoffootsoldiers; i++) {
                    footSoldiers[i] = nullptr;
                }
                countoffootsoldiers -= loss;
            }
        }
    }

    void performAction() {
        updateStandardDuties();
    }

    Noble* getCurrentLord() const { return currentLord; }
    int getCourtierCount() const { return countofcourtiers; }
    LandlessLord* getCourtier(int i) const { return (i >= 0 && i < countofcourtiers) ? courtiers[i] : nullptr; }
    float getTension() const { return civilWarTension; }
    void resetTension() { civilWarTension = 0.0f; }
    int getWealth() const { return wealth; }
    void setWealth(int w) { wealth = w; }
    int getRealmID() const { return realmID; }
    int getTerrainCode() const { return terrainCode; }

    void setCurrentLord(Noble* n) { currentLord = n; }
    


    void addFootSoldier(FootSoldier* f) {
        if (!footSoldiers) footSoldiers = new FootSoldier*[2000]; 
        if (countoffootsoldiers < 2000) footSoldiers[countoffootsoldiers++] = f;
    }
    


    void addCourtier(LandlessLord* l) {
        if (!courtiers) courtiers = new LandlessLord*[50];
        if (countofcourtiers < 50) courtiers[countofcourtiers++] = l;
    }


    void addWarriorLord(WarriorLord* w) {
        if (!warriorLords) warriorLords = new WarriorLord*[20];
        if (countofwarriorlords < 20) warriorLords[countofwarriorlords++] = w;
    }


    void addKnight(Knight* k) {
        if (!knights) knights = new Knight*[1000];
        if (countofknights < 1000) knights[countofknights++] = k;
    }



    void addCavalry(Cavalry* c) {
        if (!cavalry) cavalry = new Cavalry*[500];
        if (countofcavalry < 500) cavalry[countofcavalry++] = c;
    }


    void addRam(BatteringRam* r) {
        if (!rams) rams = new BatteringRam*[100];
        if (countoframs < 100) rams[countoframs++] = r;
    }
    
    void addCatapult(Catapult* c) {
        if (!catapults) catapults = new Catapult*[100];
        if (countofcatapults < 100) catapults[countofcatapults++] = c;
    }

    void addWarShip(WarShip* w) {
        if (!warShips) warShips = new WarShip*[200];
        if (countofships < 200) warShips[countofships++] = w;
    }


    void addSage(Sage* s) {
        if (!sages) sages = new Sage*[10];
        if (countofsages < 10) sages[countofsages++] = s;
    }



};


void terminateLord(Noble* n);


class FrostPeaksKingdom : public Kingdom {
protected:
    int coldAttritionModifier; 
public:
    FrostPeaksKingdom(int w, int d, int id, int tax) : Kingdom(w, d, id, tax), coldAttritionModifier(5) { terrainCode = 1; }

    void applyColdWeather();
    void performAction() {
        applyColdWeather();      
        updateStandardDuties(); 
    }
};

class VerdantKingdom : public Kingdom {
protected:
    int foodSurplusBonus;
public:
    VerdantKingdom(int w, int d, int id, int tax, int food) : Kingdom(w, d, id, tax), foodSurplusBonus(food) { terrainCode = 2; }

    void applyFoodBonus() {
        wealth += foodSurplusBonus;
    }

    void performAction() {
        applyFoodBonus();         
        updateStandardDuties();  
    }
};

class CoastalKingdom : public Kingdom {
protected:
    int navalTradeBonus;
public:
    CoastalKingdom(int w, int d, int id, int tax, int ntb) : Kingdom(w, d, id, tax), navalTradeBonus(ntb) { terrainCode = 3; }

    void applyTradeBonus() {
        int bonus = (baseTaxIncome * navalTradeBonus * countofships) / 100;
        wealth += bonus;
    }

    void performAction() {
        applyTradeBonus();        
        updateStandardDuties();  
    }
};

class TheVoidRift : public Kingdom {
protected:
    int threatLevel;
public:
    TheVoidRift() : Kingdom(0, 0, 0, 0), threatLevel(0) { terrainCode = 4; }

    void performAction() {
        threatLevel += 1;
        std::cout << "the void rift is growing\n";
        std::cout << "the progress is " << threatLevel << "\n";
    }

    int getThreatLevel() const { return threatLevel; }
};

class ThePeaksOfFrost : public FrostPeaksKingdom {
public:
    ThePeaksOfFrost(int w, int d, int id, int tax) : FrostPeaksKingdom(w, d, id, tax) {}
};

class TheSkyCitadel : public VerdantKingdom {
public: 
    TheSkyCitadel(int w, int d, int id, int tax, int food) : VerdantKingdom(w, d, id, tax, food) {}
};
class TheVerdantExpanse : public VerdantKingdom {
public:
    TheVerdantExpanse(int w, int d, int id, int tax, int food) : VerdantKingdom(w, d, id, tax, food) {}
};

class TheCrimsonSands : public VerdantKingdom {
public:
    TheCrimsonSands(int w, int d, int id, int tax, int food) : VerdantKingdom(w, d, id, tax, food) {}
};

class TheAzureRiverlands : public VerdantKingdom {
public:
    TheAzureRiverlands(int w, int d, int id, int tax, int food) : VerdantKingdom(w, d, id, tax, food) {}
};

class TheSunlandEmpire : public CoastalKingdom {
public:
    TheSunlandEmpire(int w, int d, int id, int tax, int ntb) : CoastalKingdom(w, d, id, tax, ntb) {}
};

class TheCorsairIsles : public CoastalKingdom {
public:
    TheCorsairIsles(int w, int d, int id, int tax, int ntb) : CoastalKingdom(w, d, id, tax, ntb) {}
};

class TheTempestCoast : public CoastalKingdom {
public:
    TheTempestCoast(int w, int d, int id, int tax, int ntb) : CoastalKingdom(w, d, id, tax, ntb) {}
};

class TheImperialSeat : public CoastalKingdom {
public:
    TheImperialSeat(int w, int d, int id, int tax, int ntb) : CoastalKingdom(w, d, id, tax, ntb) {}
};



#endif