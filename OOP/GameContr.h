#ifndef GAMECONTR_H
#define GAMECONTR_H
#include "Game.h"
#include "Command.h"
#include "ConsoleIn.h"
#include "ConsoleOut.h"
template <typename IHandler, typename OHandler>
class GameContr{
    IHandler Inner;
    OHandler Outer;
    Game* game;
    public:
    GameContr(Game* NewGame);
    ~GameContr() = default;
    void Start();
    void MainCycle();
    int PlayerMove();

};
#endif