#ifndef SHOTOOB_H
#define SHOTOOB_H

#include <exception>
#include <string>
#include <iostream>

class ShotOOB/*Short of Shot out of bounds*/ : public std::exception {
private:
    std::string Error = "Shot is out of bounds";
public:
    explicit ShotOOB();
    const char* what() const noexcept override;
};

#endif