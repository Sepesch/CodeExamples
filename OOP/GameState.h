#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "SpellBook.h"
#include "corruptedSave.h"
#include <fstream>
#include <sstream>
#include <filesystem>
class GameState{
public:
    int Fieldsize[2];
    bool quit;
    int victory = 0;
    bool failedload = 0;
    bool gameStarted = 0;
    Manager* Pmanage;
    Manager* Emanage;
    Map* Pfield;
    Map* Efield;
    SpellBook* Abilities;
    vector<int> ShipSizes;
    ~GameState() = default;
    int GetStarted(tuple<int, int> FieldSize, int NumberOfShips, vector<tuple<int, char, int, int>> Ships);
    int FleetUp(int NumberOfShips, vector<tuple<int, char, int, int>> Ships);
    void Loadgame(const string& fileName);
    friend ostream& operator<<(ostream &out, GameState &state);
    friend istream& operator>>(istream &in, GameState &state);
    void saveGame(const string& fileName) const;
    void nextOp();
    void ClearAll();
};
#endif