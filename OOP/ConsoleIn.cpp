#include "ConsoleIn.h"
#include <sstream>
#include "FileInteractionError.h"
void ConsoleIn::Init(const std::string Settings){
    try{
        
    std::ifstream fileToRead(Settings + ".txt");
    if(!fileToRead.is_open()){
        throw FileInteractionError("Cant open file" + Settings + ".txt\n");
    }
    std::string line;
    Command tmp;
    while(std::getline(fileToRead, line)){
        std::istringstream iss(line);
        char symbol;
        std::string comm;

        iss >> symbol;
        
        iss.ignore(3);
        std::getline(iss, comm);
        
        tmp = strToComm(comm);
        if(commands.count(symbol)){
            throw FileInteractionError("Duplicate key binding for symbol: " + std::string(1, symbol) + "\n");
        }
        if(reversedComms.count(tmp)){
            throw FileInteractionError("Duplicate command binding for command: " + comm + "\n");
        }
        commands[symbol] = tmp;
        reversedComms[tmp] = symbol;
    }
    fileToRead.close();
    }
    catch(std::exception& e){
        std::cout << e.what() << "\n";
        this->setDefaultComms();
    }
}
std::map<Command, char> ConsoleIn::SendComms(){
    return reversedComms;
}
void ConsoleIn::setDefaultComms(){
    this->commands.clear();
    this->commands['1'] = Command::Attack;
    this->commands['2'] = Command::UseAbility;
    this->commands['8'] = Command::Save;
    this->commands['9'] = Command::Load;
    this->commands['0'] = Command::Exit;
    this->reversedComms.clear();
    this->reversedComms[Command::Attack] = '1';
    this->reversedComms[Command::UseAbility] = '2';
    this->reversedComms[Command::Save] = '8';
    this->reversedComms[Command::Load] = '9';
    this->reversedComms[Command::Exit] = '0';
}
Command ConsoleIn::strToComm(const std::string& str){
    if (str == "Attack") return Command::Attack;
    if (str == "UseAbility") return Command::UseAbility;
    if (str == "Save") return Command::Save;
    if (str == "Load") return Command::Load;
    if (str == "Exit") return Command::Exit;
    return Command::None;
}
Command ConsoleIn::readCommand(){
    char input;
    std::cin>>input;
    return commands[input];
}
int ConsoleIn::requestSize(){
    int input;
    std::cin>>input;
    return input;
}
std::string ConsoleIn::requestFileName(){
    std::string input;
    std::cin>>input;
    return input;
}
std::tuple<int,int> ConsoleIn::requestCords(){
    int x, y;
    std::cin>> x >> y;
    return std::make_tuple(x,y);
}
std::tuple<int, char,int,int> ConsoleIn::requestShip(){
    int l, x, y;
    char d;
    std::cin>>l>>d>>x>>y;
    return std::make_tuple(l, d, x, y);
}