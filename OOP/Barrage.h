#ifndef BARRAGE_H
#define BARRAGE_H
#include "Spell.h"
class Barrage:public Spell{
public:
    bool cast(Map* map, Manager* forThisToWorkProperly, int* cords) override;
    std::string saySpell() override;
};
#endif