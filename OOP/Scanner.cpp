#include "Scanner.h"
bool Scanner::cast(Map* map, Manager* forThisToWorkProperly, int* shot){
    int *tmp = map->getSize();
    if(shot[0]<0){shot[0]=0;}
    if(shot[0]>=tmp[0]){shot[0]=tmp[0]-2;}
    if(shot[1]<0){shot[1]=0;}
    if(shot[1]>tmp[1]){shot[1]=tmp[1]-2;}
    int tmp1[2]; tmp1[0]=shot[0]+1; tmp1[1] = shot[1];
    int tmp2[2]; tmp2[1]=shot[1]+1; tmp2[0] = shot[0];
    int tmp3[2]; tmp3[0]=shot[0]+1; tmp3[1]=shot[1]+1;
    if(map->SectorChecker(shot) || map->SectorChecker(tmp1) || map->SectorChecker(tmp2) || map->SectorChecker(tmp3)){
        std::cout<<"SHIP DETECTED\n";
    }
    else{
        std::cout<<"NO SHIP";
    }
    return 0;
}
std::string Scanner::saySpell(){return "Scanner\n";}