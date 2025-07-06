#include "NoSpells.h"
    NoSpells::NoSpells(){}
    const char* NoSpells::what() const noexcept{
        return Error.c_str();
    }