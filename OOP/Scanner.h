#ifndef SCANNER_H
#define SCANNER_H
#include "Spell.h"
class Scanner:public Spell{
public:
    bool cast(Map* map, Manager* forThisToWorkProperly, int* cords) override;
    std::string saySpell() override;
};
#endif