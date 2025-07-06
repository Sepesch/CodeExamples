#include "Manager.h"
Manager::Manager(){}
void Manager::get_Fleet(vector<tuple<int, char, int, int>> Sizes){
    
    for(int i=0; i<Sizes.size(); i++){
        get_Ship(Sizes[i]);
    }
}
void Manager::get_Ship(tuple<int, char, int, int> NewShip){
    ShipInfo info(get<2>(NewShip), get<3>(NewShip));
    info.Set_ori(get<1>(NewShip));
    Report.push_back(info);
    Ship newborn=Ship(get<0>(NewShip));
    Fleet.push_back(newborn);
}
void Manager::show_Fleet(){
    for(int i=0; i<Fleet.size(); i++){
        cout<<"Stats of ship number "<<i+1<<": ";Fleet.at(i).showShip();
        cout<<Report[i].x<<"."<<Report[i].y<<" orientation-";
        if(Report[i].ori=='d'){cout<<"vertical";}
        else{cout<<"horizontal";}
        cout<<'\n';
    }
}
void Manager::Bot_Ship(int size, int* borders){
        srand(time(0));
        int tempCord; tempCord = rand()%2;
        int tempPos[2]; 
        tempPos[0] = rand()%borders[0];
        tempPos[1] = rand()%borders[1];
        ShipInfo info(tempPos[0], tempPos[1]);
            if(tempCord==0){
                info.Set_ori('d');
            }
            else if (tempCord==1)
            {
                info.Set_ori('r');
            }else{
                cout<<"ERROR";
            }
            Report.push_back(info);
            Ship newborn=Ship(size);
            Fleet.push_back(newborn);
}
bool Manager::Hit(int * coords){
    for(int i=0; i<Fleet.size(); i++){
        for(int j=0; j<Fleet.at(i).getSize(); j++){
            if(Report[i].ori=='r'){
                if(Report[i].x+j==coords[0] and Report[i].y==coords[1]){
                    if(Fleet.at(i).damage(j)){
                        return 1;
                    }
                }
            }
            else{
                if(Report[i].y+j==coords[1] and Report[i].x==coords[0]){
                    if(Fleet.at(i).damage(j)){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
bool Manager::CheckAll(){
    for(int i=0; i<Fleet.size(); i++){
        if(!Fleet[i].isDead()){
            return false;
        }
    }
    return true;
}
vector<int> Manager::getShipInfo(int* coords){
    vector<int> val;
    vector<int> No;
    for(int i=0; i<Fleet.size(); i++){
        for(int j=0; j<Fleet.at(i).getSize(); j++){
            if(Report[i].ori=='r'){
                if(Report[i].x+j==coords[0] and Report[i].y==coords[1]){
                    val.push_back(Report[i].x); val.push_back(Report[i].y); val.push_back(Fleet[i].getSize()); val.push_back(Report[i].ori);
                    return val;
                }
            }
            else{
                if(Report[i].y+j==coords[1] and Report[i].x==coords[0]){
                    val.push_back(Report[i].x); val.push_back(Report[i].y); val.push_back(Fleet[i].getSize()); val.push_back(Report[i].ori);
                    return val;
                }
            }
        }
    }
    return No;
}
void Manager::Auto_Fleet(vector<int> Sizes, int* borders){
    srand(time(0));
    for(int i=0; i<Sizes.size(); i++){
        int tempCord; tempCord = rand()%2;
        int tempPos[2]; 
        tempPos[0] = rand()%borders[0];
        tempPos[1] = rand()%borders[1];
        ShipInfo info(tempPos[0], tempPos[1]);
            if(tempCord==0){
                info.Set_ori('d');
            }
            else if (tempCord==1)
            {
                info.Set_ori('r');
            }else{
                cout<<"ERROR";
            }
            Report.push_back(info);
            Ship newborn=Ship(Sizes[i]);
            Fleet.push_back(newborn);
    }
}
vector <Ship>& Manager::send_Fleet(){
    return Fleet;
}
vector <ShipInfo>& Manager::send_Coordinates(){
    return Report;
}
string Manager::getShipInfo(int index){
    string result = to_string(Fleet[index].getSize())+" "+Report[index].ori +" "+ to_string(Report[index].x) + " " + to_string(Report[index].y) + "\n";
    return result;

}
void Manager::delete_ship(int index){
    Fleet.erase(Fleet.begin()+index);
    Report.erase(Report.begin()+index);
}
Manager::~Manager(){}
