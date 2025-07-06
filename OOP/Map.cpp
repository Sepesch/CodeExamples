#include "Map.h"
        void Map::Fill_Map(const int* cords){
            size=new int[2];
            size[0]=cords[0];
            size[1]=cords[1];
            vector<Sector> tmp;
            for(int j=0; j < size[0]; j++){
                for(int i = 0; i < size[1]; i++){
                    tmp.push_back(unknown);
                }
                field.push_back(tmp);
                FoggedField.push_back(tmp);
                tmp.clear();
            }
        }
        void Map::LoadMapLine(string line, bool which, int index){
            if(which){
                for(int i=0; i<line.length(); i++){
                    switch (line[i])
                    {
                    case '1':
                        field[i][index] = ship;
                        break;
                    case '0':
                        field[i][index] =  blank;
                    case '*':
                        field[i][index] = unknown;
                    default:
                        break;
                    }
                }
            }
            else{              
                 for(int i=0; i<line.length(); i++){
                    switch (line[i])
                    {
                    case '1':
                        FoggedField[i][index] = ship;
                        break;
                    case '0':
                        FoggedField[i][index] =  blank;
                    case '*':
                        FoggedField[i][index] = unknown;
                    default:
                        break;
                    }
                }}
        }
        std::vector<std::vector<Sector>> Map::Show(){
            return field;
        }
        std::vector<std::vector<Sector>> Map::ShowFogged(){
            return FoggedField;
        }
        bool Map::CheckShip(ShipInfo boat, int length){
            int X=boat.x;
            int Y=boat.y;
            for(int i=0; i<length; i++){
                if(boat.ori == 'r'){
                    if (i+X>=size[0] || i+X<0 || Y>=size[1] || Y<0){
                        return false;
                    }
                    if (0<=X+1+i && X+1+i<size[0]){
                        if(field[X+1+i][Y]==ship){return false;}}
                    if (i==0){
                        if(X-1>=0){
                            if(field[X-1][Y]==ship){return false;}}}
                    if (0<=X-1 && X-1<size[0]){
                        if(field[X+i][Y-1]==ship){return false;}}
                    if (0<=X+1 && X+1<size[0]){
                        if(field[X+i][Y+1]==ship){return false;}}
                }
                else{
                    if (i+Y>=size[1] || i+Y<0 || X>=size[0] || X<0){ 
                        return false;
                    }
                    if (0<=Y+1+i && Y+1+i<size[1]){
                        if(field.at(X).at(Y+i+1)==ship){return false;}}
                    if (i==0){
                        if(X-1>=0){
                            if(field.at(X)[Y-1]==ship){return false;}}}
                    if (0<=X-1 && X-1<size[0]){ 
                        if(field[X-1][Y+i]==ship){return false;}}                       
                    if (0<=X+1 && X+1<size[0]){
                        if(field[X+1][Y+i]==ship){return false;}}
                }
            }
            return true;
        }
        void Map::ShipIsNoMore(vector<int> info){
            int X=info[0];
            int Y=info[1];
            for(int i=0; i<info[2]; i++){
                if(info[3]==100){
                    if (i+X>=size[0] || i+X<0 || Y>=size[1] || Y<0){
                        return;
                    }
                    if (0<=X+1+i && X+1+i<size[0] && i==info[2]-1){field[X+1+i][Y]=blank;FoggedField[X+1+i][Y]=blank;}

                    if (i==0 && X-1>=0){field[X-1][Y]=blank;FoggedField[X+1+i][Y]=blank;}

                    if (0<=X-1 && X-1<size[0]){field[X+i][Y-1]=blank;FoggedField[X+1+i][Y]=blank;}

                    if (0<=X+1 && X+1<size[0]){field[X+i][Y+1]=blank;FoggedField[X+1+i][Y]=blank;}
                }
                else{
                    if (i+Y>=size[1] || i+Y<0 || X>=size[0] || X<0){
                        return;
                    }
                    if (0<=Y+1+i && Y+1+i<size[1] && i==info[2]-1){field[X][Y+1+i]=blank;FoggedField[X+1+i][Y]=blank;}

                    if (i==0 && Y-1>=0){field[X][Y-1]=blank;FoggedField[X+1+i][Y]=blank;}

                    if (0<=Y-1 && Y-1<size[1]){field[X-1][Y+i]=blank;FoggedField[X+1+i][Y]=blank;}

                    if (0<=Y+1 && Y+1<size[1]){field[X+1][Y+i]=blank;FoggedField[X+1+i][Y]=blank;}
                }
            }
        }
        bool Map::PlaceShip(Ship boat, ShipInfo info){
            if(CheckShip(info, boat.getSize())){
                
                if(info.ori == 'r'){
                    int j=info.y;
                    for(int k=0; k<boat.getSize() && 0<=k<size[0] && 0<=j<size[1]; k++){
                        field[k+info.x][j]=ship;
                    }
                }
                else{
                    int j=info.x;
                    for(int k=0; k<boat.getSize() && 0<=k<size[0] && 0<=j<size[1]; k++){
                        field[j][k+info.y]=ship;
                    }
                }
                }
                else{
                    return 0;
                }
            return 1;
        }
        bool Map::SectorChecker(int *cords){
            if (field[cords[0]][cords[1]]==ship){
                return true;
            }
            else{
                return false;
            }
        }
        void Map::ActivateDD(){
            DD=1;
        }
        bool Map::Hit(int *coords, Manager* manage){
            try{
            if((0<=coords[0] && coords[0]<size[0]) && (0<=coords[1] && coords[1]<size[1])){
                if(field[coords[0]][coords[1]]==ship){
                    if (DD==1){manage->Hit(coords);DD=0;}  
                    if(manage->Hit(coords)){
                        ShipIsNoMore(manage->getShipInfo(coords));
                        return 1;
                    }
                }
                if(field[coords[0]][coords[1]]==unknown){
                    FoggedField[coords[0]][coords[1]]=blank;
                    field[coords[0]][coords[1]]=blank;
                }
                else{
                    FoggedField[coords[0]][coords[1]] = field[coords[0]][coords[1]];
                }

            }
            else{throw ShotOOB();}
            }
            catch(const ShotOOB& e){
                cerr<<e.what()<<endl;
            }
            return 0;

        }
        Map& Map::operator=(const Map& source){
            if(this==&source){
                return *this;
            }
            size=source.size;
            if (source.field.empty()){
                vector<vector<Sector>> field;
                vector<Sector> tmp;
                for(int j=0; j < size[0]; j++){
                    for(int i = 0; i < size[1]; i++){
                        tmp.push_back(source.field[j][i]);
                    }
                    field.push_back(tmp);
                }
            }
    return *this;
}
int * Map::getSize(){
    return size;
}
string Map::getMapLine(int index, bool which){
    string result;
    if(which){
        for(int j=0; j<size[0]; j++){
            if(field.at(j).at(index)==unknown){result = result + "*";}
            if(field.at(j).at(index)==ship){result = result + "1";}
            if(field.at(j).at(index)==blank){result = result + "0";}
        }
    }
    else{
        for(int j=0; j<size[0]; j++){
            if(FoggedField.at(j).at(index)==unknown){result = result + "*";}
            if(FoggedField.at(j).at(index)==ship){result = result + "1";}
            if(FoggedField.at(j).at(index)==blank){result = result + "0";}
        }
    }
    return result + "\n";

}
Map& Map::operator=(Map&& moved){
    if(this==&moved){
        return *this;
    }
    field=moved.field;
    size=moved.size;
    return *this;
}
Map::~Map(){}  