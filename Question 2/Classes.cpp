#include "Classes.h"

void Asset::takeDmg(int damage) {
  int dr = damage - armor;
  int loss = (dr > 0) ? dr : 0;
  hp -= loss;
  if (hp < 0) hp = 0;
}

void Asset::takedefaultDmg(int damage) {
  hp -= damage;
  if (hp < 0)
    hp = 0;
}

void FrostPeaksKingdom::applyColdWeather() {
  for (int i = 0; i < countoffootsoldiers; i++)
    if (footSoldiers[i])
      footSoldiers[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofknights; i++)
    if (knights[i])
      knights[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofcavalry; i++)
    if (cavalry[i])
      cavalry[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofwarriorlords; i++)
    if (warriorLords[i])
      warriorLords[i]->Knight::takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countoframs; i++)
    if (rams[i])
      rams[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofcatapults; i++)
    if (catapults[i])
      catapults[i]->takedefaultDmg(coldAttritionModifier);
  for (int i = 0; i < countofships; i++)
    if (warShips[i])
      warShips[i]->Transport::takedefaultDmg(coldAttritionModifier);
}

Kingdom::~Kingdom() {
      delete[] footSoldiers; delete[] knights;
      delete[] cavalry;
      delete[] rams;
      delete[] catapults;
      delete[] warShips;
      delete[] courtiers; delete[] warriorLords;
      delete[] sages;
      delete[] heirIds;
}

void terminateLord(Noble *n) {
  if (!n) return;
  
  switch(n->getTypeCode()) {
    case 1:
        delete (LandedLord *)n;
        break;
    case 2:
        delete (LandlessLord *)n;
        break;
    case 3:
        delete (WarriorLord *)n;
        break;
    default:
        delete n;
        break;
  }
}