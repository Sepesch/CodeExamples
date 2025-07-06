#include "Barrage.h"
bool Barrage::cast(Map* map, Manager* forThisToWorkProperly, int* cords){
    std::vector<Ship> tmp = forThisToWorkProperly->send_Fleet();
    if(tmp.size()>0){
        int a = rand()%tmp.size(); int b = rand()%tmp.at(a).getSize();
        if(tmp.at(a).damage(b)){
            return 1;
        }
    }
    return 0;
}
std::string Barrage::saySpell(){return "Barrage\n";}