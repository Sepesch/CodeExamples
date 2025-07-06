#ifndef SPELL_H
#define SPELL_H
#include "Map.h"
class Spell{
public:
    virtual bool cast(Map* map,Manager* forThisToWorkProperly, int* cords)=0;
    virtual std::string saySpell()=0;
};
#endif