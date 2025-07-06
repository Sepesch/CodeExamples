#ifndef GAME_H
#define GAME_H
#include "SpellBook.h"
#include "GameState.h"
#include "Command.h"
class Game{
    GameState cond;
public:
    Game() = default;
    int newGame(tuple<int, int> FieldSize, int NumberOfShips, vector<tuple<int, char, int, int>> Ships);
    int MainCycle();
    void PlayerMove(Command action);
    void AIMove();
    void Shoot(tuple<int, int> cords);
    std::tuple<std::vector<std::vector<Sector>>, std::vector<std::vector<Sector>>> ShowBattlefields();
    void useSpell(tuple<int, int> cords);
    void Exit();
    void NewShip();
    void victory();
    bool CheckSpells();
    void SaveGame(string filename);
    void LoadGame(string filename);
    void Restart();
    ~Game() = default;
};
#endif