#include "ShotOOB.h"
    ShotOOB::ShotOOB(){}
    const char* ShotOOB::what() const noexcept{
        return Error.c_str();
    }