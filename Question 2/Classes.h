//$$just ensure the engine always calls the most-derived destructor explicitly.
#ifndef CLASSES_HEADER
#define CLASSES_HEADER

#include <iostream>


// BASE CLASSES
class Entity {
protected:
    int id;
    char* name; 
public:
    Entity(int i, const char* n) : id(i) {
        int len = 0;
        while(n[len] != '\0') len++;
        name = new char[len + 1];
        for(int j = 0; j <= len; j++) name[j] = n[j];
    }
    ~Entity() { delete[] name; }
};

class Person : public Entity {
protected:
    int age;
    bool isAlive;
public:
    Person(int i, const char* n, int a) : Entity(i, n), age(a), isAlive(true) {} // may change default value of is alive
};

class Asset : public Entity {
protected:
    int hp;
    int maxHp;
    int armor;
    int maintenanceCost;
public:
    Asset(int i, const char* n, int h, int mH, int ar, int cost) : Entity(i, n), hp(h), maxHp(mH), armor(ar), maintenanceCost(cost) {}
    // void takeDmg(int damage){
    //     int damagereceived = damage - armor; if(damagereceived<0) damagereceived=0;
    //     hp -=damagereceived;
    //     if(hp<0) hp=0;  
    // }
    // void takedefaultDmg(int damage){
    //     hp -= damage;
    //     if(hp<0) hp=0;
    // } // $$ This just might be faulty
};


//PERSON INHERITENCE

class MilitaryUnit : public Person {
protected:
    int attackPower;
public:
    MilitaryUnit(int i, const char* n, int a, int ap) : Person(i, n, a), attackPower(ap) {}
};

class MeleeUnit : public MilitaryUnit {
protected:
    bool servesFrontline;
public:
    MeleeUnit(int i, const char* n, int a, int ap, bool frontline) : MilitaryUnit(i, n, a, ap), servesFrontline(frontline) {}
};

class FootSoldier : public MeleeUnit {
public:
    FootSoldier(int i, const char* n, int a, int ap, bool frontline) : MeleeUnit(i, n, a, ap, frontline) {}
};

class Knight : public MeleeUnit {
protected:
    bool shieldBlock;
public:
    Knight(int i, const char* n, int a, int ap, bool frontline, bool shield) : MeleeUnit(i, n, a, ap, frontline), shieldBlock(shield) {}
};

class MountedUnit : public MilitaryUnit {
protected:
    float chargeMultiplier;
    float terrainPenalty;
public:
    MountedUnit(int i, const char* n, int a, int ap, float cm, float tp) : MilitaryUnit(i, n, a, ap), chargeMultiplier(cm), terrainPenalty(tp) {}
};

class Cavalry : public MountedUnit {
public:
    Cavalry(int i, const char* n, int a, int ap, float cm, float tp) : MountedUnit(i, n, a, ap, cm, tp) {}
};

// The Council
class Sage : public Person {
protected:
    int healingPower;
    int wisdom;
public:
    Sage(int i, const char* n, int a, int hpw, int wis) : Person(i, n, a), healingPower(hpw), wisdom(wis) {}
};

class Assassin : public Person {
protected:
    int stealthStat;
    int clientIdx;
public:
    Assassin(int i, const char* n, int a, int stealth, int client) : Person(i, n, a), stealthStat(stealth), clientIdx(client) {}
};

//ASSET INHERITENCE

class SiegeEngine : public Asset {
protected:
    int siegeDamage;
    int speed;
public:
    SiegeEngine(int i, const char* n, int h, int mH, int ar, int cost, int sdmg, int spd) : Asset(i, n, h, mH, ar, cost), siegeDamage(sdmg), speed(spd) {}
};

class BatteringRam : public SiegeEngine {
public:
    BatteringRam(int i, const char* n, int h, int mH, int ar, int cost, int sdmg, int spd) : SiegeEngine(i, n, h, mH, ar, cost, sdmg, spd) {}
};

class Catapult : public SiegeEngine {
protected:
    int aeDamage;
    int aoeTargets;
public:
    Catapult(int i, const char* n, int h, int mH, int ar, int cost, int sdmg, int spd, int aoeD, int aoeT) : SiegeEngine(i, n, h, mH, ar, cost, sdmg, spd), aeDamage(aoeD), aoeTargets(aoeT) {}
};

class Transport : public Asset {
protected:
    int troopCapacity;
    int knotsSpeed;
public:
    Transport(int i, const char* n, int h, int mH, int ar, int cost, int capacity, int knots) : Asset(i, n, h, mH, ar, cost), troopCapacity(capacity), knotsSpeed(knots) {}
};

//THE NOBLES
class Noble : public Person {
protected:
    int realmIdx;
    int ambitionStat;
    int diplomacyStat;
    float healthMeter;
    int fatherId;
    int* childrenIds; // this is the array of childrenids
    int childCount;
public:
    Noble(int i, const char* n, int a, int rIdx, int amb, int diplo, int fId, int* cIds, int cCount) : Person(i, n, a), realmIdx(rIdx), ambitionStat(amb), diplomacyStat(diplo), healthMeter(0.0f), fatherId(fId), childCount(cCount) {
        if (cCount > 0) {
            childrenIds = new int[cCount];
            for(int j = 0; j < cCount; j++) childrenIds[j] = cIds[j];
        }
        else {
            childrenIds = nullptr;
        }
    }
    ~Noble() { delete[] childrenIds; }
};

class LandedLord : public Noble {
protected:
    int strategyStat;
public:
    LandedLord(int i, const char* n, int a, int rIdx, int amb, int diplo, int fId, int* cIds, int cCount, int strat) : Noble(i, n, a, rIdx, amb, diplo, fId, cIds, cCount), strategyStat(strat) {}
};

class LandlessLord : public Noble {
protected:
    float assassinationPlotProgress;
public:
    LandlessLord(int i, const char* n, int a, int rIdx, int amb, int diplo, int fId, int* cIds, int cCount) : Noble(i, n, a, rIdx, amb, diplo, fId, cIds, cCount), assassinationPlotProgress(0.0f) {}
};


//MULTI LINEAGE

class WarriorLord : public Noble, public Knight {
public:
    WarriorLord(/* noble values -->>*/int id, const char* n, int a, int rIdx, int amb, int diplo, int fId, int* cIds, int cCount,/* knight values -->>*/int ap, bool frontline, bool shield) : Noble(id, n, a, rIdx, amb, diplo, fId, cIds, cCount), Knight(id, n, a, ap, frontline, shield) {}

    // Example of handling the ambiguity:
    // int getNobleId() const { return Noble::id; }
    // int getKnightId() const { return Knight::id; }
};

class WarShip : public Transport, public SiegeEngine {
public:
    WarShip(/* transport values -->>*/int id, const char* n, int h, int mH, int ar, int cost, int capacity, int knots, /* siegeEngine values -->>*/ int sdmg, int spd) : Transport(id, n, h, mH, ar, cost, capacity, knots), SiegeEngine(id, n, h, mH, ar, cost, sdmg, spd) {}

    void takeHullDamage(int dmg) {
        int damagetaken= dmg - Transport::armor; if(damagetaken<0) damagetaken=0;
        Transport::hp -= damagetaken;
    }

    void takeWeaponDamage(int dmg) {
        int damagetaken= dmg - SiegeEngine::armor; if(damagetaken<0) damagetaken=0;
        SiegeEngine::hp -= damagetaken;
    }
};


////////////////////////////////////////////////////////////////////
////////////////////////The Realms and their Holdings///////////////////////
//////////////////////////////////////////////////////////////////////

class Kingdom {
protected:
    int wealth;
    int defenseStat;
    int baseTaxIncome;

    LandedLord* currentLord;
    int* heirIds;
    int heirCount;
    LandlessLord** courtiers    ;
    int courtierCount;

    FootSoldier** footSoldiers;
    int footSoldierCount;
    Knight** knights;
    int knightCount;
    Cavalry** cavalry;
    int cavalryCount;
    BatteringRam** rams;
    int ramCount;
    Catapult** catapults;
    int catapultCount;
    WarShip** warShips;
    int warShipCount;
    WarriorLord** warriorLords; // When doing succession search I will also have to serach warrior lords in addition to landless lords
    int warriorLordCount;

    Sage** sages;
    int sageCount;

public:
    Kingdom(int w, int d, int tax) : wealth(w), defenseStat(d), baseTaxIncome(tax), currentLord(nullptr), heirIds(nullptr), heirCount(0), courtiers(nullptr), courtierCount(0),footSoldiers(nullptr), footSoldierCount(0), knights(nullptr), knightCount(0),cavalry(nullptr), cavalryCount(0), rams(nullptr), ramCount(0), catapults(nullptr), catapultCount(0), warShips(nullptr), warShipCount(0),warriorLords(nullptr), warriorLordCount(0), sages(nullptr), sageCount(0) {}

    void performAction() {
        collectTaxes();
        ageSubjects();
    }

    void collectTaxes() {
        wealth += baseTaxIncome;
    }

    void ageSubjects() {
        // $$ Logic for aging would go here in the engine phase //check alive and age
    }
};

class FrostPeaksKingdom : public Kingdom {
protected:
    int coldAttritionModifier; 
public:
    FrostPeaksKingdom(int w, int d, int tax) : Kingdom(w, d, tax), coldAttritionModifier(5) {} //5 is default value for cold attrition modifier

    void applyColdWeather() {
        // // 1. Foot Soldiers
        // for (int i = 0; i < footSoldierCount; i++) {
        //     if (footSoldiers[i]) {
        //         footSoldiers[i]->takedefaultDmg(coldAttritionModifier);
        //     }
        // }

        // // 2. Knights
        // for (int i = 0; i < knightCount; i++) {
        //     if (knights[i]) {
        //         knights[i]->takeRawDamage(coldAttritionModifier);
        //     }
        // }

        // // 3. Cavalry
        // for (int i = 0; i < cavalryCount; i++) {
        //     if (cavalry[i]) {
        //         cavalry[i]->takeRawDamage(coldAttritionModifier);
        //     }
        // }

        // // 4. Warrior Lords (The Hybrid Case)
        // for (int i = 0; i < warriorLordCount; i++) {
        //     if (warriorLords[i]) {
        //         // We use the Knight path to reach the Asset-based HP
        //         // because WarriorLord inherits Asset through the Knight branch
        //         warriorLords[i]->Knight::takeRawDamage(coldAttritionModifier);
        //     }
        // }
    }

    void performAction() {
        applyColdWeather();
        Kingdom::performAction(); 
    }
};

class VerdantKingdom : public Kingdom {
protected:
    int foodSurplusBonus;
public:
    VerdantKingdom(int w, int d, int tax, int food) : Kingdom(w, d, tax), foodSurplusBonus(food) {}

    void applyFoodBonus() {
        wealth += foodSurplusBonus;
    }

    void performAction() {
        applyFoodBonus();
        Kingdom::performAction();
    }
};

class CoastalKingdom : public Kingdom {
protected:
    int navalTradeBonus;
public:
    CoastalKingdom(int w, int d, int tax, int ntb) : Kingdom(w, d, tax), navalTradeBonus(ntb) {}

    void applyTradeBonus() {
        // The formula is give::: baseTaxIncome * navalTradeBonus * shipCount / 100
        int bonus = (baseTaxIncome * navalTradeBonus * warShipCount) / 100;
        wealth += bonus;
    }

    void performAction() {
        applyTradeBonus();
        Kingdom::performAction();
    }
};

class TheVoidRift : public Kingdom {
protected:
    int threatLevel;
public:
    TheVoidRift() : Kingdom(0, 0, 0), threatLevel(0) {}

    void performAction() {
        threatLevel += 1;
        // Void Rift ignores nomral logic... it only represents thread level
    }
};

///////Specific subregions within regions//////
//////// Frost Peaks//////
class ThePeaksOfFrost : public FrostPeaksKingdom {
public:
    ThePeaksOfFrost(int w, int d, int tax) : FrostPeaksKingdom(w, d, tax) {}
};

//////// Verdant Realms //////
class TheSkyCitadel : public VerdantKingdom {
public: 
    TheSkyCitadel(int w, int d, int tax, int food) : VerdantKingdom(w, d, tax, food) {}
};
class TheVerdantExpanse : public VerdantKingdom {
public:
    TheVerdantExpanse(int w, int d, int tax, int food) : VerdantKingdom(w, d, tax, food) {}
};

class TheCrimsonSands : public VerdantKingdom {
public:
    TheCrimsonSands(int w, int d, int tax, int food) : VerdantKingdom(w, d, tax, food) {}
};

class TheAzureRiverlands : public VerdantKingdom {
public:
    TheAzureRiverlands(int w, int d, int tax, int food) : VerdantKingdom(w, d, tax, food) {}
};

//////// Coastal Realms//////
class TheSunlandEmpire : public CoastalKingdom {
public:
    TheSunlandEmpire(int w, int d, int tax, int ntb) : CoastalKingdom(w, d, tax, ntb) {}
};

class TheCorsairIsles : public CoastalKingdom {
public:
    TheCorsairIsles(int w, int d, int tax, int ntb) : CoastalKingdom(w, d, tax, ntb) {}
};

class TheTempestCoast : public CoastalKingdom {
public:
    TheTempestCoast(int w, int d, int tax, int ntb) : CoastalKingdom(w, d, tax, ntb) {}
};

class TheImperialSeat : public CoastalKingdom {
public:
    TheImperialSeat(int w, int d, int tax, int ntb) : CoastalKingdom(w, d, tax, ntb) {}
};



#endif