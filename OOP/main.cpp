#include "GameContr.h"
#include "ConsoleIn.h"
#include "ConsoleOut.h"
#include "GameContr.cpp"

int main(){
    Game game;
    GameContr <ConsoleIn, ConsoleOut> Match (&game);
    Match.Start();
}