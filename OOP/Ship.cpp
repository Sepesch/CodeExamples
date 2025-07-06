#include "Ship.h"
Ship::Ship(int len){
    for (int i = 0; i<len; i++){
        segHp.push_back(untouched);
    }
}
bool Ship::damage(int index){
        if(segHp.at(index)==damaged){segHp.at(index)=destroyed;}
        if(segHp.at(index)==untouched){segHp.at(index)=damaged;}
        for(int i=0; i<segHp.size(); i++){
            if(segHp.at(i)!=destroyed){
                return 0;
            }
           
        } 
        if(cond==1){return 0;}
        cond=1;
        return cond;
    }
void Ship::showShip(){
    if (cond){std::cout<<"alive"<<" len-"<<std::to_string(segHp.size())<<" position-";}
    else{std::cout<<"dead"<<" len-"<<std::to_string(segHp.size())<<" position-";}
    }
int Ship::getSize(){
        return segHp.size();
    }
bool Ship::isDead(){return cond;}
Ship::~Ship(){}