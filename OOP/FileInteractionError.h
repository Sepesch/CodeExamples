
#ifndef FILEINTERACTIONERROR_H
#define FILEINTERACTIONERROR_H
#include <iostream>

class FileInteractionError : public std::runtime_error {
public:
    FileInteractionError(const std::string &msg) : std::runtime_error(msg){};
};


#endif