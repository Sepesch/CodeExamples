#ifndef CORRUPTEDSAVE_H
#define CORRUPTEDSAVE_H

#include <exception>
#include <string>
#include <iostream>

class corruptedSave : public std::exception {
private:
    std::string Error = "Safe file is corrupted. Unable to load game";
public:
    explicit corruptedSave();
    const char* what() const noexcept override;
};

#endif