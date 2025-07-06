#include "ConsoleOut.h"
void ConsoleOut::GetComms(std::map<Command, char> Comms){
    Commands = Comms;
}
void ConsoleOut::ShowFields(Game* game){
    std::tuple<std::vector<std::vector<Sector>>, std::vector<std::vector<Sector>>> BattleFields = game->ShowBattlefields();
    std::string result;
    std::vector<std::vector<Sector>> field = get<0>(BattleFields);
    std::vector<std::vector<Sector>> FoggedFields = get<1>(BattleFields);
    int size[2];
    size[0] = field[0].size();
    size[1] = field.size();
    for(int index = 0; index<size[1]; index++){
        result = result + "| ";
    for(int j=0; j<size[0]; j++){
        if(field.at(j).at(index)==unknown){result = result + "*";}
        if(field.at(j).at(index)==ship){result = result + "1";}
        if(field.at(j).at(index)==blank){result = result + "0";}
        }
        result = result + " |\n";
    }
    cout<<result;
    for(int i=0; i<size[0]+4; i++){cout<<"=";}
    cout<<endl;
    result.clear();
        for(int index = 0; index<size[1]; index++){
        result = result + "| ";
    for(int j=0; j<size[0]; j++){
        if(FoggedFields.at(j).at(index)==unknown){result = result + "*";}
        if(FoggedFields.at(j).at(index)==ship){result = result + "1";}
        if(FoggedFields.at(j).at(index)==blank){result = result + "0";}
        }
        result = result + " |\n";
    }
    cout<<result;
}
void ConsoleOut::FieldSize(){
    cout<<"ENTER SIZE OF THE BATTLEFIELD\n";
}
void ConsoleOut::ShipsInitFirst(){
    cout<<"ENTER NUMBER OF SHIPS\n";
}
void ConsoleOut::ShipsInitSecond(){
    cout<<"ENTER SIZE, DIRECTION AND POSITION OF EACH SHIP\n";
}
void ConsoleOut::ImproperShip(int a){
    cout<<"Ship number "<<a+1<<" has imporper position\n"<<"Please enter new size, direction and coordinates for the ship:\n";
}
void ConsoleOut::Start(){
    cout<<"new game - "<<Commands[Command::Attack]<<", load from file - "<<Commands[Command::Load]<<endl;
}
void ConsoleOut::Spell(Game* game){
    if(game->CheckSpells()){
        cout<<"No available abilities\n";
    }
}
void ConsoleOut::Attack(){
    cout<<"Enter coordinates for an Attack\n";
}
void ConsoleOut::VOD(int state){
    if(state == 1){
        cout<<"Congatulations! You won!\n";
    }
    if(state == 2){
        cout<<"You lost! Good luck next time\n";
    }
}
void ConsoleOut::File(){
    cout << "Type file name\n";
}
void ConsoleOut::Actions(){
    cout<<"Choose action\n";

    cout<<"Attack - "<<Commands[Command::Attack]<<", UseAbility - "<<Commands[Command::UseAbility];
    cout<<", Save game - "<<Commands[Command::Save]<<", Load form a file - "<<Commands[Command::Load];
    cout<<", Exit - "<<Commands[Command::Exit]<<endl;
}