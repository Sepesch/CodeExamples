#include "GameState.h"
int GameState::GetStarted(tuple<int, int> Size, int NumberOfShips, vector<tuple<int, char, int, int>> Ships){
    gameStarted = 1;
    quit = false;
    Fieldsize[0] = get<0>(Size);
    Fieldsize[1] = get<1>(Size);
    
    this->Pfield = new Map();
    this->Efield = new Map();
    
    Pfield->Fill_Map(Fieldsize);
    Efield->Fill_Map(Fieldsize);

    this->Abilities = new SpellBook();
    this->Pmanage = new Manager();

    Abilities->getSpells(Efield, Emanage);

    for(int i=0; i<NumberOfShips; i++){
        ShipSizes.push_back(get<0>(Ships[i]));
    }
    int tmp = FleetUp(NumberOfShips, Ships);
    if(tmp!=0){return tmp;}
    this->Emanage = new Manager();
    Emanage->Auto_Fleet(ShipSizes, Fieldsize);
    for(int i=0; i<ShipSizes.size(); i++){
        if(!Efield->PlaceShip(Emanage->send_Fleet()[i], Emanage->send_Coordinates()[i])){
            Emanage->delete_ship(i);
            Emanage->Bot_Ship(ShipSizes[i], Fieldsize);
            i--;
        }
    }
return 0;
}
int GameState::FleetUp(int NumberOfShips, vector<tuple<int, char, int, int>> Ships){
    Pmanage->get_Fleet(Ships);
    for (int i=0; i<ShipSizes.size(); i++){
        try{
        if(!Pfield->PlaceShip(Pmanage->send_Fleet()[i], Pmanage->send_Coordinates()[i])){throw ImproperShipPlacement(i);}
        }
        catch(const ImproperShipPlacement& e){
            Pmanage->delete_ship(i);
            return i;
            Pmanage->get_Ship(Ships[i]);
            i--;
        }
    }
    return 0;
}

ostream &operator<<(ostream &out, const GameState &state) {
    cout << "Begin save\n";
    out << "Game Settings\n";
    out << to_string(state.Fieldsize[0]) + " " + to_string(state.Fieldsize[1])+ " " + to_string(state.Pmanage->send_Fleet().size())+"\n";
    out << "Player ships\n";
    for (int i = 0; i < state.ShipSizes.size(); ++i) {
        out << state.Pmanage->getShipInfo(i);
    }
    out << "Computer Ships\n";
    for (int i = 0; i < state.ShipSizes.size(); ++i) {
        out << state.Emanage->getShipInfo(i);
    }
    out << "Player map\n";
    for (int i = 0; i < state.Fieldsize[1]; ++i) {
        out << state.Pfield->getMapLine(i, 1);
    }
    out << "Player field\n";
    for (int i = 0; i < state.Fieldsize[1]; ++i) {
        out << state.Pfield->getMapLine(i, 0);
    }
    out<<"Computer map\n";
    for (int i = 0; i < state.Fieldsize[1]; ++i) {
        out << state.Efield->getMapLine(i, 1);
    }
    out << "Computer field\n";
    for (int i = 0; i < state.Fieldsize[1]; ++i) {
        out << state.Efield->getMapLine(i, 0);
    }
    out << "Ability\n";
    out << state.Abilities->AmountoSpells() << "\n";
    out << state.Abilities->AllSpellsInStr();

    return out;
}
istream& operator>>(istream &in, GameState &state){
    GameState &temporary = *new GameState;
    string  line;
    int tmp;
    in.ignore(256, '\n');
    in >> temporary.Fieldsize[0] >> temporary.Fieldsize[1] >> tmp;

    temporary.Pmanage = new Manager;
    getline(in, line);
    getline(in, line);
    try{
    if(line != "Player ships"){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }

    temporary.ShipSizes.clear();
    int inf[3];
    char dir;
    for(int i=0; i<tmp; i++){
        in >> inf[0] >> dir >> inf[1] >> inf[2];
        try{
        if(inf[0]>4 || inf[0]<1 || inf[1]>=temporary.Fieldsize[0] || inf[1]<0 || inf[2]<0 || inf[2]>=temporary.Fieldsize[1] || !(dir == 'r' || dir == 'd')){throw corruptedSave();}
        }
        catch(const corruptedSave& e){
            cout<<e.what()<<endl;
            state.failedload = 1;
            return in;
        }
        temporary.Pmanage->get_Ship(make_tuple(inf[0], dir, inf[1], inf[2]));
        temporary.ShipSizes.push_back(inf[0]);
    }
    


    temporary.Emanage = new Manager;
    in.ignore(256, '\n');
    getline(in, line);
    try{
    if(line != "Computer Ships"){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }

    for(int i=0; i<tmp; i++){
        in >> inf[0] >> dir >> inf[1] >> inf[2];
        try{
        if(inf[0]>4 || inf[0]<1 || inf[1]>=temporary.Fieldsize[0] || inf[1]<0 || inf[2]<0 || inf[2]>=temporary.Fieldsize[1] || !(dir == 'r' || dir == 'd')){throw corruptedSave();}
        }
        catch(const corruptedSave& e){
            cout<<e.what()<<endl;
            state.failedload = 1;
            return in;
        }
        temporary.Emanage->get_Ship(make_tuple(inf[0], dir, inf[1], inf[2]));
    }



    in.ignore(256, '\n');
    temporary.Pfield = new Map();
    getline(in, line);
    try{
    if(line != "Player map"){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }

    temporary.Pfield->Fill_Map(temporary.Fieldsize);
    for(int i=0; i<temporary.Fieldsize[1]; i++){
        getline(in, line);
    try{
    if(line.length() != temporary.Fieldsize[0]){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }
        temporary.Pfield->LoadMapLine(line, true, i);
    }



    getline(in, line);
    try{
    if(line != "Player field"){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }
    
    for(int i=0; i<temporary.Fieldsize[1]; i++){
        getline(in, line);
    try{
    if(line.length() != temporary.Fieldsize[0]){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }
        temporary.Pfield->LoadMapLine(line, false, i);
    }



    temporary.Efield = new Map();
    temporary.Efield->Fill_Map(temporary.Fieldsize);
    getline(in, line);
    try{
    if(line != "Computer map"){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }
    for(int i=0; i<temporary.Fieldsize[1]; i++){
        getline(in, line);
        try{
    if(line.length() != temporary.Fieldsize[0]){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }
        temporary.Efield->LoadMapLine(line, 1, i);
    }


    getline(in, line);
    try{
    if(line != "Computer field"){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }
    for(int i=0; i<temporary.Fieldsize[1]; i++){
        getline(in, line);
        try{
    if(line.length() != temporary.Fieldsize[0]){throw corruptedSave();}
    }
    catch(const corruptedSave& e){
        cout<<e.what()<<endl;
        state.failedload = 1;
        return in;
    }
        temporary.Efield->LoadMapLine(line, 0, i);
    }

    getline(in, line);
    in>>tmp;
    getline(in, line);
    temporary.Abilities = new SpellBook;
    for(int i=0; i<tmp; i++){
        getline(in, line);
        temporary.Abilities->LoadSpell(line);
    }
    temporary.Abilities->update(temporary.Efield, temporary.Emanage);
    if(state.gameStarted){state.ClearAll();}
    state = temporary;
    return in;
    }
void GameState::saveGame(const string& fileName) const {


    ofstream fileToWrite(fileName + ".txt");
    fileToWrite << *this;
    fileToWrite.close();
}
void GameState::Loadgame(const string& fileName){
    ifstream fileToRead(fileName + ".txt");
    fileToRead >> *this;
    fileToRead.close();
}
void GameState::nextOp(){
    delete this->Emanage;
    delete this->Efield;
    this->Efield = new Map();
    Efield->Fill_Map(Fieldsize);
    this->Emanage = new Manager();
    Emanage->Auto_Fleet(ShipSizes, Fieldsize);
    for(int i=0; i<ShipSizes.size(); i++){
        if(!Efield->PlaceShip(Emanage->send_Fleet()[i], Emanage->send_Coordinates()[i])){
            Emanage->delete_ship(i);
            Emanage->Bot_Ship(ShipSizes[i], Fieldsize);
            i--;
        }
    }
}
void GameState::ClearAll(){
    delete this->Abilities;
    delete this->Pfield;
    delete this->Efield;
    delete this->Pmanage;
    delete this->Emanage;
    victory = 0;
    quit = false;
}