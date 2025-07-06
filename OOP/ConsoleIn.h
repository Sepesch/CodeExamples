#ifndef CONSOLEIN_H
#define CONSOLEIN_H
#include "Command.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
class ConsoleIn{
    std::map<char, Command> commands;
    std::map<Command, char> reversedComms;
    void setDefaultComms();
    Command strToComm(const std::string& str);
    public:
    ConsoleIn()=default;
    void Init(const std::string Settings);
    ~ConsoleIn()=default;
    Command readCommand();
    int requestSize();
    std::map<Command, char> SendComms();
    std::string requestFileName();
    std::tuple<int,int> requestCords();
    std::tuple<int, char,int,int> requestShip();
};

#endif