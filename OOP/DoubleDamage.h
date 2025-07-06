#ifndef DOUBLEDAMAGE_H
#define DOUBLEDAMAGE_H
#include "Spell.h"
class DoubleDamage:public Spell{
public:
    bool cast(Map* map, Manager* forThisToWorkProperly, int* cords) override;
    std::string saySpell()override;
};
#endif