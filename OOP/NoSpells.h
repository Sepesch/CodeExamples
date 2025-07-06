#ifndef NOSPELLS_H
#define NOSPELLS_H

#include <exception>
#include <string>

class NoSpells : public std::exception {
private:
    std::string Error="No available abilities";
public:
    explicit NoSpells();
    const char* what() const noexcept override;
};

#endif