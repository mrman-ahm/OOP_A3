#ifndef Q2_SUBMISSION_H
#define Q2_SUBMISSION_H

///////////////////////////////////////////
//////////////This file was supposed to be custom_string.h//////
///////////////////////////////////////////
#ifndef STRING_HEADER
#define STRING_HEADER




inline int getLength(const char* str) {
    if(str == nullptr)  return 0;
    
    int length=0;
    while(*(str++)!='\0')
        length++;
    return length;
}

inline char* copyString(const char* str) {

    if(str == nullptr)
        return nullptr;
    int length = getLength(str);

    char* copy = new char[length+1];
    for(int i=0; i<length;i++){
        *(copy+i)=*(str+i);
    }
    *(copy+length)='\0';
    return copy;
} 

inline bool areEqual(const char* str1, const char* str2) {
    int length1=getLength(str1);
    int length2=getLength(str2);
    if(length1==0&&length2==0)    return 0;
    if(length1!=length2)    return 0;
    for(int i=0; i<length1;i++){

        if(*(str1+i)!=*(str2+i))
            return 0;
    }
    return 1;
    
}

inline bool isSmallerThan(const char* str1, const char* str2) {
    if(str1 == nullptr || str2 == nullptr) return 0;
    
    int i = 0;
    while(*(str1 + i) != '\0' && *(str2 + i) != '\0') {
        if(*(str1 + i) < *(str2 + i)) return 1;
        if(*(str1 + i) > *(str2 + i)) return 0;
        i++;
    }
    
    return (*(str1 + i) == '\0' && *(str2 + i) != '\0');
}

inline char* concatenate(const char* str1, const char* str2) {
    
    int length1=getLength(str1);
    int length2=getLength(str2);
    if(length1+length2==0) return nullptr;
    char* concatenated = new char[length1+length2+1];
    
    int index =0;
    for(int i=0; i<length1+length2;i++){
        if(i<length1&&str1!=nullptr){
            *(concatenated+index) = *(str1+i);
            index++;
        }
        else if(str2!=nullptr){
            *(concatenated+index) = *(str2+i-length1);
            index++;
        }   
    }
    *(concatenated+length1+length2) = '\0';


    return concatenated;
}

inline void toUpperCase(char* str) {
    if(str == nullptr)  return;

    int length = getLength(str);
    for(int i=0; i<length; i++){
        if(*(str+i)>='a'&&*(str+i)<='z'){
            *(str+i) -= 32;
            
        }
    }
}

inline void toLowerCase(char* str) {
    if(str == nullptr)  return;

    int length = getLength(str);
    for(int i=0; i<length; i++){
        if(*(str+i)>='A'&&*(str+i)<='Z'){
            *(str+i) += 32;
            
        }
    }    
}

inline int findFirstOccurrence(const char* str, char c) {
    if(str== nullptr)   return -1;
    int length = getLength(str);
    for(int i=0;i<length;i++){
        if(*(str+i)==c)
            return i;
    }
    return -1;
    
}

inline int findSubstring(const char* str, const char* sub) {
    if(str==nullptr||sub==nullptr) return -1;
    int lengthstr=getLength(str);
    int lengthsub=getLength(sub);
    if(lengthsub==0) return 0;
    if(lengthsub>lengthstr) return -1;

    for(int i=0;i<=lengthstr-lengthsub;i++){
        bool found=1;
        for(int j=0;j<lengthsub;j++){
            if(*(str+i+j)!=*(sub+j)){
                found=0;
                break;
            }
        }
        if(found) return i;
    }
    return -1;
    
}

inline char* reverseString(char* str){
    if(str==nullptr) return 0;
    int length=getLength(str);
    char* rev = new char[length+1];
    for(int i=0;i<length;i++){
        *(rev+i) = *(str+length -i-1);
    }
    *(rev + length) = '\0';
    return rev;
}

inline char* removeCharacter(char* str, char c) {
    if(str==nullptr) return 0;
    int length=getLength(str);
    int index=0;
    char* arr_del = new char[length+1];
    for(int i=0; i<length; i++){
        if(*(str+i)!=c){
        *(arr_del+index++) = *(str+i);
        }
    }
    *(arr_del+index) = '\0';
    return arr_del;
}

inline char* substring(const char* str, int start, int length) {
    int total_length = getLength(str);
    if(str==nullptr || start<0 || length<=0||start>=total_length||start+length>total_length) return nullptr;

    char* substr=new char[length+1];
    for(int i=0;i<length;i++){
        *(substr+i)=*(str+start+i);
    }
    *(substr+length)='\0';
    return substr;
}

inline char* replaceChar(char* str, char oldChar, char newChar){
    if(str==nullptr) return nullptr;
    int length=getLength(str);
    char* newstr = new char[length+1];
    for(int i=0;i<length;i++){
        if(*(str+i)==oldChar)
            *(newstr+i)=newChar;
        else 
            *(newstr+i) = *(str+i);
    }
    *(newstr + length ) = '\0';

    return newstr;
}


inline char* insertAt(const char* str, const char* sub, int pos) {
    int length=getLength(str);
    if(pos>length) return nullptr;
    int sublength = getLength(sub);
    char* newstr= new char[length+sublength+1];
    int index =0;
    for(int i=0; i<length; i++){
        if(i==pos){
            for(int j=0; j<sublength; j++){
                *(newstr + index++)=*(sub+j);
            }
        }
        *(newstr + index++)=*(str+i);
    }
    *(newstr+length+sublength)='\0';
    return newstr;
}

inline char* deleteRange(char* str, int start, int length) {
    int total_length=getLength(str);
    if(total_length<=length+start) length = total_length-start;
    if(start<0||length<=0)  return str;
    
    char* newstr= new char[total_length-length+1];
    int index =0;
    for(int i=0; i<total_length; i++){
        if(i==start)
        i+=length;
    *(newstr + index++)=*(str+i);
}
*(newstr+index)='\0';
return newstr;    
}


inline char* replaceSubstring(const char* str, const char* oldSub, const char* newSub) {
    if(str == nullptr || oldSub == nullptr || newSub == nullptr) return nullptr;
    
    int length = getLength(str);
    int oldSublength = getLength(oldSub);
    int newSublength = getLength(newSub);
    
    if(length == 0 || oldSublength == 0) return copyString(str);


    int count = 0;  
    for(int i = 0; i <= length-oldSublength; i++){
        if(findSubstring(str + i, oldSub) == 0){
            count++;
            i+=oldSublength-1;
        }
    }

    char* result = new char[length+count*(newSublength-oldSublength)+1];
    int j = 0, i = 0;
    
    while(i < length) {
        if(i <= length - oldSublength && findSubstring(str + i, oldSub) == 0) {
            for(int k = 0; k < newSublength; k++) *(result + j++) = *(newSub + k);
            i += oldSublength;
        } else {
            *(result + j++) = *(str + i++);
        }
    }
    
    *(result + j) = '\0';
    return result;
}

inline char* compress(const char* str) {
    if(str == nullptr) return nullptr;
    int len = getLength(str);
    if(len == 0) return nullptr;
    
    char* result = new char[len * 2 + 1];
    int j = 0;
    
    for(int i = 0; i < len; ) {
        char ch = *(str+i);
        int count = 1;
        while(i + count < len && *(str+i + count) == ch) count++;
        
        *(result+j++) = ch;
        *(result+j++)= '0' + count; 
        i += count;
    }
    
    *(result+j) = '\0';
    return result;
}

inline char* decompress(const char* str) {
    if(str == nullptr) return nullptr;
    int length = getLength(str);
    if(length == 0) return nullptr;
    
    char* newstr = new char[length * 10+1]; 
    int j = 0;
    
    for(int i = 0; i < length; i += 2) {
        char ch = *(str+i);          
        int count = *(str+i + 1) - '0'; 
        
        for(int k = 0; k < count; k++) {
            *(newstr+j++) = ch;         
        }
    }
    
    *(newstr+j) = '\0';
    return newstr;
}

inline char* rotateString(char* str, int k){
    if(str == nullptr) return nullptr;
    int length = getLength(str);
    if(length == 0) return nullptr;
    k=k%length;
    if(k<0) k+=length;

    char* newstr = new char[length+1];
    for(int i=0; i<length; i++){
        *(newstr + i) = *(str + i);
    }
    *(newstr+length) = '\0';
    for(int i=0; i<k; i++){
        char last = *(newstr);
        for(int j=0; j<length-1; j++){
            
            *(newstr+j)=*(newstr+j+1);
        }
        *(newstr+length-1) = last;
    }
    return newstr;
}

inline int  stoi(const char* str) {
    if (str == nullptr) return 0;
    
    int result = 0;
    int sign = 1;
    int i = 0;

    while (*(str + i) == ' ') i++;

    if (*(str + i) == '-') {
        sign = -1;
        i++;
    } else if (*(str + i) == '+') {
        i++;
    }

    while (*(str + i) >= '0' && *(str + i) <= '9') {
        result = result * 10 + (*(str + i) - '0');
        i++;
    }

    return result * sign;
}

inline float stof(const char* str) {
    if (str == nullptr) return 0.0f;

    float result = 0.0f;
    float sign = 1.0f;
    int i = 0;

    while (*(str + i) == ' ') i++;

    if (*(str + i) == '-') {
        sign = -1.0f;
        i++;
    } else if (*(str + i) == '+') {
        i++;
    }

    while (*(str + i) >= '0' && *(str + i) <= '9') {
        result = result * 10.0f + (*(str + i) - '0');
        i++;
    }

    if (*(str + i) == '.') {
        i++;
        float factor = 0.1f;
        while (*(str + i) >= '0' && *(str + i) <= '9') {
            result += (*(str + i) - '0') * factor;
            factor /= 10.0f;
            i++;
        }
    }

    return result * sign;
}

#endif

///////////////////////////////////////////
//////////////This file was supposed to be Classes.h//////
///////////////////////////////////////////
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
    
    int* childrenIds; 
    friend struct WarHours;
    friend struct InevitableFate;
    friend void terminateLord(Noble* n);

public:
    int realmIdx;
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
    int getMaintenanceCost() const { return Knight::getMaintenanceCost(); }
};

class WarShip : public Transport, public SiegeEngine {
public:
    WarShip(int id, const char* n, int h, int mH, int ar, int cost, int capacity, int knots, int sdmg, int spd) : Transport(n, id, h, mH, ar, cost, capacity, knots), SiegeEngine(n, id, h, mH, ar, cost, sdmg, spd) {}
    int getMaintenanceCost() const { return Transport::getMaintenanceCost(); }

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
        for (int i = this->countoffootsoldiers; i < 2000; i++) this->footSoldiers[i] = nullptr;
        parent.countoffootsoldiers -= this->countoffootsoldiers;

        this->countofknights = parent.countofknights / 2;
        this->knights = new Knight*[1000];

        for (int i = 0; i < this->countofknights; i++) {
            this->knights[i] = parent.knights[parent.countofknights - 1 - i];
            parent.knights[parent.countofknights - 1 - i] = nullptr;
        }
        for (int i = this->countofknights; i < 1000; i++) this->knights[i] = nullptr;
        parent.countofknights -= this->countofknights;

        this->countofcavalry = parent.countofcavalry / 2;
        this->cavalry = new Cavalry*[500];

        for (int i = 0; i < this->countofcavalry; i++) {
            this->cavalry[i] = parent.cavalry[parent.countofcavalry - 1 - i];
            parent.cavalry[parent.countofcavalry - 1 - i] = nullptr;
        }
        for (int i = this->countofcavalry; i < 500; i++) this->cavalry[i] = nullptr;
        parent.countofcavalry -= this->countofcavalry;



        this->countofships = parent.countofships / 2;
        this->warShips = new WarShip*[200];
        for (int i = 0; i < this->countofships; i++) {
            this->warShips[i] = parent.warShips[parent.countofships - 1 - i];
            parent.warShips[parent.countofships - 1 - i] = nullptr;
        }
        for (int i = this->countofships; i < 200; i++) this->warShips[i] = nullptr;
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
        if (!footSoldiers) {
            footSoldiers = new FootSoldier*[2000];
            for(int i=0; i<2000; i++) footSoldiers[i] = nullptr;
        }
        if (countoffootsoldiers < 2000) footSoldiers[countoffootsoldiers++] = f;
    }
    


    void addCourtier(LandlessLord* l) {
        if (!courtiers) {
            courtiers = new LandlessLord*[50];
            for(int i=0; i<50; i++) courtiers[i] = nullptr;
        }
        if (countofcourtiers < 50) courtiers[countofcourtiers++] = l;
    }


    void addWarriorLord(WarriorLord* w) {
        if (!warriorLords) {
            warriorLords = new WarriorLord*[20];
            for(int i=0; i<20; i++) warriorLords[i] = nullptr;
        }
        if (countofwarriorlords < 20) warriorLords[countofwarriorlords++] = w;
    }


    void addKnight(Knight* k) {
        if (!knights) {
            knights = new Knight*[1000];
            for(int i=0; i<1000; i++) knights[i] = nullptr;
        }
        if (countofknights < 1000) knights[countofknights++] = k;
    }



    void addCavalry(Cavalry* c) {
        if (!cavalry) {
            cavalry = new Cavalry*[500];
            for(int i=0; i<500; i++) cavalry[i] = nullptr;
        }
        if (countofcavalry < 500) cavalry[countofcavalry++] = c;
    }


    void addRam(BatteringRam* r) {
        if (!rams) {
            rams = new BatteringRam*[100];
            for(int i=0; i<100; i++) rams[i] = nullptr;
        }
        if (countoframs < 100) rams[countoframs++] = r;
    }
    
    void addCatapult(Catapult* c) {
        if (!catapults) {
            catapults = new Catapult*[100];
            for(int i=0; i<100; i++) catapults[i] = nullptr;
        }
        if (countofcatapults < 100) catapults[countofcatapults++] = c;
    }

    void addWarShip(WarShip* w) {
        if (!warShips) {
            warShips = new WarShip*[200];
            for(int i=0; i<200; i++) warShips[i] = nullptr;
        }
        if (countofships < 200) warShips[countofships++] = w;
    }


    void addSage(Sage* s) {
        if (!sages) {
            sages = new Sage*[10];
            for(int i=0; i<10; i++) sages[i] = nullptr;
        }
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

///////////////////////////////////////////
//////////////This file was supposed to be InevitableFate.h//////
///////////////////////////////////////////
#ifndef INEVITABLE_FATE_H
#define INEVITABLE_FATE_H


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

///////////////////////////////////////////
//////////////This file was supposed to be WarHours.h//////
///////////////////////////////////////////
#ifndef WAR_HOURS_H
#define WAR_HOURS_H

#include <iostream>

struct WarHours {
    Kingdom* attacker;
    Kingdom* defender;
    

    int presentHour; 
    WarHours(Kingdom* atk, Kingdom* def) : attacker(atk), defender(def), presentHour(0) {}


    int figureOutWhoWins();

    void doPostWarStuff(Kingdom* attacker, Kingdom* defender, int result, int** relations, int countofrealms);



private:


    void killTheUnits(Kingdom* target, int incomingDamage);


    int generateLineDamage(Kingdom* k, float strategyMod);

    int getFootSoldiersTotalHP(Kingdom* k);

    int generateHealing(Kingdom* k, int incomingInfantryDamage);

    int calculateTotalPower(Kingdom* k);




    int generateSiegeDamage(Kingdom* k, float strategyMod);

    int generatePhaseDamage(Kingdom* k, float strategyMod, int phaseType);

    float getStrategyModifier(Kingdom* k);


    void applyNavalAttrition(Kingdom* target);
    
    int generateFlankDamage(Kingdom* k, float strategyMod);
    
    int generateEliteDamage(Kingdom* k, float strategyMod);



};




#endif

///////////////////////////////////////////
//////////////This file was supposed to be ScenarioLoader.h//////
///////////////////////////////////////////
#ifndef SCENARIO_LOADER_H
#define SCENARIO_LOADER_H

class Aethelgard;

class ScenarioLoader {
public:

    static bool load(const char* csvPath, Aethelgard* engine);

    
};

#endif

///////////////////////////////////////////
//////////////This file was supposed to be Aethelgard.h//////
///////////////////////////////////////////
#ifndef AETHELGARD_H
#define AETHELGARD_H







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

#endif
