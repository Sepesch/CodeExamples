#ifndef SHIP_H
#define SHIP_H
#include <iostream>
#include <vector>
using namespace std;
enum Health{untouched, damaged, destroyed};
class Ship{
    private:
        bool cond=0;
        std::vector <Health> segHp;
    public:
        Ship(int len);
        bool damage(int index);
        void showShip();
        int getSize();
        bool isDead();
        ~Ship();
};
#endif //SHIP.H