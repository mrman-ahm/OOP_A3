//$$just ensure the engine always calls the most-derived destructor explicitly.
#ifndef CLASSES_HEADER
#define CLASSES_HEADER

#include <iostream>


// BASE CLASSES
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
    // Engine-accessible getters
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


//THe MILITARY

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

// The Council
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
public:
    Assassin(const char* n, int i, int a, int stealth, int client) : Person(n, i, a), clientIdx(client), stealthStat(stealth) {}
};

//ASSET INHERITENCE

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
    int typeCode; // 1=Landed, 2=Landless, 3=WarriorLord
    int strategyStat;
    int ambitionStat;
    float healthMeter;
    int diplomacyStat;
    int realmIdx;
    int* childrenIds; // this is the array of childrenids
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


//MULTI LINEAGE

class WarriorLord : public Noble, public Knight {
public:
    WarriorLord(/* common params -->>*/const char* n, int id, int a, /* noble side -->>*/int cCount, int fId, int strat, int amb, int diplo, int rIdx, int* cIds, /* knight side -->>*/int h, int mH, int ar, int cost, int ap, bool frontline, bool shield) : Noble(n, 3, id, a, cCount, fId, strat, amb, diplo, rIdx, cIds), Knight(n, id, h, mH, ar, cost, ap, frontline, shield) {}

    // Example of handling the ambiguity:
    // int getNobleId() const { return Noble::id; }
    // int getKnightId() const { return Knight::id; }
};

class WarShip : public Transport, public SiegeEngine {
public:
    WarShip(/* transport values -->>*/int id, const char* n, int h, int mH, int ar, int cost, int capacity, int knots, /* siegeEngine values -->>*/ int sdmg, int spd) : Transport(n, id, h, mH, ar, cost, capacity, knots), SiegeEngine(n, id, h, mH, ar, cost, sdmg, spd) {}

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
    float civilWarTension;
    int realmID;

    int footSoldierCount;
    FootSoldier** footSoldiers;

    int catapultCount;
    Catapult** catapults;

    int warriorLordCount;
    WarriorLord** warriorLords; 

    int wealth;
    Noble* currentLord;

    int knightCount;
    Knight** knights;

    int sageCount;
    Sage** sages;

    int baseTaxIncome;
    
    int cavalryCount;
    Cavalry** cavalry;

    int defenseStat;

    int ramCount;
    BatteringRam** rams;

    int heirCount;
    int* heirIds;

    int courtierCount;
    LandlessLord** courtiers;

    int warShipCount;
    WarShip** warShips;

    friend struct WarHours;
    friend struct InevitableFate;
public:
    Kingdom(int w, int d, int id, int tax) : civilWarTension(0.0f), realmID(id), footSoldierCount(0), footSoldiers(nullptr), catapultCount(0), catapults(nullptr), warriorLordCount(0), warriorLords(nullptr), wealth(w), currentLord(nullptr), knightCount(0), knights(nullptr), sageCount(0), sages(nullptr), baseTaxIncome(tax), cavalryCount(0), cavalry(nullptr), defenseStat(d), ramCount(0), rams(nullptr), heirCount(0), heirIds(nullptr), courtierCount(0), courtiers(nullptr), warShipCount(0), warShips(nullptr) {}

    ~Kingdom(); // Destruction logic moved to Engine; this only cleans local pointer arrays.

    // PDF Section 2.4.2: The Faction Split Constructor
    // Constructs a new rebel realm by halving the parent's resources and migrating half the units.
    Kingdom(Kingdom& parent, LandlessLord* rebelLeader, int newID) : civilWarTension(0.0f), realmID(newID) {
        // 1. Initial Halving (Wealth, Tax, Defense)
        this->wealth = parent.wealth / 2; parent.wealth -= this->wealth;
        this->baseTaxIncome = parent.baseTaxIncome / 2; parent.baseTaxIncome -= this->baseTaxIncome;
        this->defenseStat = parent.defenseStat / 2; parent.defenseStat -= this->defenseStat;

        // 2. Promotion
        this->currentLord = (Noble*)rebelLeader;
        if (this->currentLord) this->currentLord->setAlive(true);

        // 3. Pointer Migration (Exactly 50% per array)
        // FootSoldiers
        this->footSoldierCount = parent.footSoldierCount / 2;
        this->footSoldiers = new FootSoldier*[this->footSoldierCount + 10]; // Buffer for growth
        for (int i = 0; i < this->footSoldierCount; i++) {
            this->footSoldiers[i] = parent.footSoldiers[parent.footSoldierCount - 1 - i];
            parent.footSoldiers[parent.footSoldierCount - 1 - i] = nullptr;
        }
        parent.footSoldierCount -= this->footSoldierCount;

        // Knights
        this->knightCount = parent.knightCount / 2;
        this->knights = new Knight*[this->knightCount + 10];
        for (int i = 0; i < this->knightCount; i++) {
            this->knights[i] = parent.knights[parent.knightCount - 1 - i];
            parent.knights[parent.knightCount - 1 - i] = nullptr;
        }
        parent.knightCount -= this->knightCount;

        // ... repeat pattern for others ...
        this->cavalryCount = parent.cavalryCount / 2;
        this->cavalry = new Cavalry*[this->cavalryCount + 10];
        for (int i = 0; i < this->cavalryCount; i++) {
            this->cavalry[i] = parent.cavalry[parent.cavalryCount - 1 - i];
            parent.cavalry[parent.cavalryCount - 1 - i] = nullptr;
        }
        parent.cavalryCount -= this->cavalryCount;

        this->warShipCount = parent.warShipCount / 2;
        this->warShips = new WarShip*[this->warShipCount + 5];
        for (int i = 0; i < this->warShipCount; i++) {
            this->warShips[i] = parent.warShips[parent.warShipCount - 1 - i];
            parent.warShips[parent.warShipCount - 1 - i] = nullptr;
        }
        parent.warShipCount -= this->warShipCount;

        // Catapults
        this->catapultCount = parent.catapultCount / 2;
        this->catapults = new Catapult*[this->catapultCount + 5];
        for (int i = 0; i < this->catapultCount; i++) {
            this->catapults[i] = parent.catapults[parent.catapultCount - 1 - i];
            parent.catapults[parent.catapultCount - 1 - i] = nullptr;
        }
        parent.catapultCount -= this->catapultCount;

        // Rams
        this->ramCount = parent.ramCount / 2;
        this->rams = new BatteringRam*[this->ramCount + 5];
        for (int i = 0; i < this->ramCount; i++) {
            this->rams[i] = parent.rams[parent.ramCount - 1 - i];
            parent.rams[parent.ramCount - 1 - i] = nullptr;
        }
        parent.ramCount -= this->ramCount;

        // Initialize empty arrays for remaining categories
        this->warriorLordCount = 0; this->warriorLords = nullptr;
        this->sageCount = 0; this->sages = nullptr;
        this->courtierCount = 0; this->courtiers = nullptr;
        this->heirCount = 0; this->heirIds = nullptr;
    }

    // PDF Table I: Net Wealth = GrossWealth - SUM(UnitCount * MaintenanceCost)
    int calculateMaintenance() {
        int total = 0;
        for (int i = 0; i < footSoldierCount; i++) if (footSoldiers[i]) total += footSoldiers[i]->getMaintenanceCost();
        for (int i = 0; i < knightCount; i++) if (knights[i]) total += knights[i]->getMaintenanceCost();
        for (int i = 0; i < cavalryCount; i++) if (cavalry[i]) total += cavalry[i]->getMaintenanceCost();
        for (int i = 0; i < ramCount; i++) if (rams[i]) total += rams[i]->getMaintenanceCost();
        for (int i = 0; i < catapultCount; i++) if (catapults[i]) total += catapults[i]->getMaintenanceCost();
        for (int i = 0; i < warShipCount; i++) if (warShips[i]) total += warShips[i]->Transport::getMaintenanceCost();
        return total;
    }

    // Step 2-4: Standard Universal Tasks
    void updateStandardDuties() {
        // Tax Collection
        wealth += baseTaxIncome;
        
        // Maintenance
        wealth -= calculateMaintenance();

        // Bankruptcy Check (Section 3 Phase 1)
        if (wealth < 0) {
            wealth = 0;
            // PDF: invoke takeCasualties() against FootSoldier array (5% loss)
            int loss = footSoldierCount * 0.05f; // floor implied by int cast
            if (loss > 0) {
                // To comply with ownership, we simply reduce the count. 
                // The Engine will handle the actual entity cleanup based on IDs.
                footSoldierCount -= loss;
            }
        }
    }

    // Default base action (can be hidden by subclasses)
    void performAction() {
        updateStandardDuties();
    }

    // ---- Engine-accessible getters (no friend class needed) ----
    Noble*        getCurrentLord()            const { return currentLord; }
    int           getCourtierCount()          const { return courtierCount; }
    LandlessLord* getCourtier(int i)          const { return (i >= 0 && i < courtierCount) ? courtiers[i] : nullptr; }
    float         getTension()                const { return civilWarTension; }
    void          resetTension()                    { civilWarTension = 0.0f; }
    int           getWealth()                 const { return wealth; }
    void          setWealth(int w)                  { wealth = w; }
    int           getRealmID()                const { return realmID; }

    void setCurrentLord(Noble* n) { currentLord = n; }
    
    void addFootSoldier(FootSoldier* f) {
        if (!footSoldiers) footSoldiers = new FootSoldier*[100]; // Basic capacity for test
        if (footSoldierCount < 100) footSoldiers[footSoldierCount++] = f;
    }
    
    void addCourtier(LandlessLord* l) {
        if (!courtiers) courtiers = new LandlessLord*[20];
        if (courtierCount < 20) courtiers[courtierCount++] = l;
    }

    void addWarriorLord(WarriorLord* w) {
        if (!warriorLords) warriorLords = new WarriorLord*[10];
        if (warriorLordCount < 10) warriorLords[warriorLordCount++] = w;
    }

    void addKnight(Knight* k) {
        if (!knights) knights = new Knight*[500];
        if (knightCount < 500) knights[knightCount++] = k;
    }

    void addCavalry(Cavalry* c) {
        if (!cavalry) cavalry = new Cavalry*[200];
        if (cavalryCount < 200) cavalry[cavalryCount++] = c;
    }

    void addSage(Sage* s) {
        if (!sages) sages = new Sage*[5];
        if (sageCount < 5) sages[sageCount++] = s;
    }
};


// Global termination helper to fix the "No-Virtual" polymorphic leak
void terminateLord(Noble* n);

class FrostPeaksKingdom : public Kingdom {
protected:
    int coldAttritionModifier; 
public:
    FrostPeaksKingdom(int w, int d, int id, int tax) : Kingdom(w, d, id, tax), coldAttritionModifier(5) {} 

    void applyColdWeather();
    void performAction() {
        applyColdWeather();      // 1. Environmental
        updateStandardDuties(); // 2-4. Universal
    }
};

class VerdantKingdom : public Kingdom {
protected:
    int foodSurplusBonus;
public:
    VerdantKingdom(int w, int d, int id, int tax, int food) : Kingdom(w, d, id, tax), foodSurplusBonus(food) {}

    void applyFoodBonus() {
        wealth += foodSurplusBonus;
    }

    void performAction() {
        applyFoodBonus();         // 1. Environmental
        updateStandardDuties();  // 2-4. Universal
    }
};

class CoastalKingdom : public Kingdom {
protected:
    int navalTradeBonus;
public:
    CoastalKingdom(int w, int d, int id, int tax, int ntb) : Kingdom(w, d, id, tax), navalTradeBonus(ntb) {}

    void applyTradeBonus() {
        // Trade Bonus calculation (Section 2.3)
        int bonus = (baseTaxIncome * navalTradeBonus * warShipCount) / 100;
        wealth += bonus;
    }

    void performAction() {
        applyTradeBonus();        // 1. Environmental
        updateStandardDuties();  // 2-4. Universal
    }
};

class TheVoidRift : public Kingdom {
protected:
    int threatLevel;
public:
    TheVoidRift() : Kingdom(0, 0, 0, 0), threatLevel(0) {}

    void performAction() {
        threatLevel += 1;
        // Void Rift ignores normal logic... it only represents threat level
    }

    int getThreatLevel() const { return threatLevel; }
};

///////Specific subregions within regions//////
//////// Frost Peaks//////
class ThePeaksOfFrost : public FrostPeaksKingdom {
public:
    ThePeaksOfFrost(int w, int d, int id, int tax) : FrostPeaksKingdom(w, d, id, tax) {}
};

//////// Verdant Realms //////
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

//////// Coastal Realms//////
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