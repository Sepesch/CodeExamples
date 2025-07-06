#ifndef IMPROPERSHIPPLACEMENT_H
#define IMPROPERSHIPPLACEMENT_H

#include <exception>
#include <string>
#include <iostream>

class ImproperShipPlacement : public std::exception {
private:
    std::string Error;
    std::string first="Ship number ";
    std::string second = " Overlaps with another ship or goes beyond the field";
public:
    explicit ImproperShipPlacement(int index);
    const char* what() const noexcept override;
};

#endif