#include "ImproperShipPlacement.h"
        ImproperShipPlacement::ImproperShipPlacement(int index){
        Error.append(first); 
        Error.append(std::to_string(index));
        Error.append(second);
        }
    const char* ImproperShipPlacement::what() const noexcept{
        return Error.c_str();
    }