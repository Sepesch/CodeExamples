#include "corruptedSave.h"
corruptedSave::corruptedSave(){}
const char* corruptedSave::what() const noexcept{
    return Error.c_str();
}