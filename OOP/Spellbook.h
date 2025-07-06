#ifndef SPELLBOOK_H
#define SPELLBOOK_H
#include "DoubleDamage.h"
#include "Scanner.h"
#include "Barrage.h"
#include "NoSpells.h"
#include "Map.h"
#include <iterator>
#include <random>
#include <vector>
#include <memory>
#include <algorithm>
class SpellBook{
private:
    std::vector<std::unique_ptr<Spell>> queue;
    Manager* manage;
    Map* field;
public:
    SpellBook() = default;
    void getSpells(Map* newMap, Manager* newManager);
    void addSpell();
    std::string AllSpellsInStr();
    void nextSpell();
    void useSpell(tuple<int, int> cords);
    int AmountoSpells();
    void LoadSpell(std::string name);
    void update(Map* newMap, Manager* newManager);
};
#endif