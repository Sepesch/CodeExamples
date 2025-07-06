#ifndef MANAGER_H
#define MANAGER_H
#include <iostream>
#include <vector>
#include "Ship.h"
#include "ShipInfo.h"
class Manager{
    private:
        vector <Ship> Fleet;
        vector <ShipInfo> Report;
    public:
        Manager();
        void get_Fleet(vector<tuple<int, char, int, int>> Sizes);
        void get_Ship(tuple<int, char, int, int> NewShip);
        void show_Fleet();
        void Bot_Ship(int num, int* borders);
        bool Hit(int * coords);
        bool CheckAll();
        vector<int> getShipInfo(int* coords);
        void Auto_Fleet(vector<int> Sizes, int* borders);
        vector <Ship>& send_Fleet();
        vector <ShipInfo>& send_Coordinates();
        string getShipInfo(int index);
        void delete_ship(int index);
        ~Manager();
};
#endif