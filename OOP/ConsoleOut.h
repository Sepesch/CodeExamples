#ifndef CONSOLEOUT_H
#define CONSOLEOUT_H
#include "Command.h"
#include "Game.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
class ConsoleOut{
    std::map<Command, char> Commands;
    public:
    ConsoleOut()=default;
    ~ConsoleOut()=default;
    void GetComms(std::map<Command, char> Comms);
    void Attack();
    void File();
    void ShowFields(Game* game);
    void FieldSize();
    void ShipsInitFirst();
    void ShipsInitSecond();
    void ImproperShip(int a);
    void Start();
    void Spell(Game* game);
    void VOD(int state);
    void Actions();
};

#endif