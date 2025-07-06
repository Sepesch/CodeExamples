#include "GameContr.h"
using namespace std;
template <typename IHandler, typename OHandler>
GameContr<IHandler, OHandler>::GameContr(Game* NewGame){
    game = NewGame;
    Inner.Init("Settings");
    Outer.GetComms(Inner.SendComms());
}

template <typename IHandler, typename OHandler>
void GameContr<IHandler, OHandler>::Start(){
    Outer.Start();
    Command starter = Inner.readCommand();
    if(starter == Command::Attack){
        Outer.FieldSize();
        std::tuple<int, int> FieldSize = Inner.requestCords();
        Outer.ShipsInitFirst();
        int NumberOfShips = Inner.requestSize();
        Outer.ShipsInitSecond();
        std::vector<tuple<int, char, int, int>> Ships;
        for(int i=0; i<NumberOfShips; i++){
            Ships.push_back(Inner.requestShip());

        }
        int ship = game->newGame(FieldSize, NumberOfShips, Ships);
        while(ship!=0){
                Outer.ImproperShip(ship);
                Ships.push_back(Inner.requestShip());
                ship = game->newGame(FieldSize, NumberOfShips, Ships);
        }
        MainCycle();
    }
    if(starter == Command::Load){
        
        game->LoadGame(Inner.requestFileName());
        MainCycle();
    }
}
template <typename IHandler, typename OHandler>
void GameContr<IHandler, OHandler>::MainCycle(){
    int tmp = 0;
    while(tmp == 0){
        Outer.ShowFields(game);
        if(PlayerMove()){return;}
        tmp = game->MainCycle();
        Outer.VOD(tmp);
        if(tmp == 1){
            game->victory(); 
            tmp=0;
        }
    }
   
} 

template <typename IHandler, typename OHandler> 
int GameContr<IHandler, OHandler>::PlayerMove(){
    bool flag = 1;
    while(flag){
    Outer.Spell(game);
    Outer.Actions();
    Command action = Inner.readCommand();
    switch (action)
    {
    case Command::Attack:
        Outer.Attack();
        game->Shoot(Inner.requestCords());
        return 0;
        break;
    
    case Command::UseAbility:
        game->useSpell(Inner.requestCords());
        break;
    
    case Command::Save:
        game->SaveGame(Inner.requestFileName());
        break;
    
    case Command::Load:
        game->LoadGame(Inner.requestFileName());
        break;
    case Command::Exit:
        game->Exit();
        return 1;
        break;
    default:
        break;
    }
    }
    return 0;
}