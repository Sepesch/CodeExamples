#include "Game.h"

int Game:: newGame(tuple<int, int> FieldSize, int NumberOfShips, vector<tuple<int, char, int, int>> Ships){
    return cond.GetStarted(FieldSize, NumberOfShips, Ships);
}
int Game::MainCycle(){
    AIMove();
    if(cond.Emanage->CheckAll()){
        return 1;
    }
    if(cond.Pmanage->CheckAll()){
        return 2;
    }
    return 0;
}
void Game::victory(){
    cond.nextOp();
    cond.victory = 0;
}
void Game::Shoot(tuple<int, int> cords){
    int shot[2];
    shot[0] = get<0>(cords);
    shot[1] = get<1>(cords);
    if(cond.Efield->Hit(shot, cond.Emanage)){
        cond.Abilities->addSpell();
    }
}
bool Game::CheckSpells(){
    if(cond.Abilities->AmountoSpells()>=1){cond.Abilities->nextSpell(); return false;}
    else{
        return true;
    }
}
void Game::Exit(){
    cond.quit = true;
}
void Game::useSpell(tuple<int, int> cords){
    cond.Abilities->useSpell(cords);
}
void Game::AIMove(){
    int x = cond.Pfield->getSize()[0];
    int y = cond.Pfield->getSize()[1];
    int shot[2];
    shot[0] = rand()%x; shot[1] = rand()%y;
    cond.Pfield->Hit(shot, cond.Pmanage);
}
std::tuple<std::vector<std::vector<Sector>>, std::vector<std::vector<Sector>>> Game::ShowBattlefields(){
    return std::make_tuple(cond.Pfield->Show(), cond.Efield->ShowFogged());
}
void Game::SaveGame(string filename){
    cond.saveGame(filename);
}
void Game::LoadGame(string filename){
    cond.Loadgame(filename);
    if(cond.failedload){
        if(cond.gameStarted){
            MainCycle();
        }
        else{
            return;
        }
    }
}
void Game::Restart(){
    cond.ClearAll();
}