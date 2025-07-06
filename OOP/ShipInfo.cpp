#include "ShipInfo.h"
ShipInfo::ShipInfo(int hor, int ver){
    x=hor;
    y=ver;
}
void ShipInfo::Set_ori(char dir){
    ori=dir;
}
ShipInfo::~ShipInfo(){}