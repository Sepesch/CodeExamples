#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <vector>
#include "Manager.h"
#include "ImproperShipPlacement.h"
#include "ShotOOB.h"
enum Sector{blank, ship, unknown};
class Map{
    private:
        vector<vector<Sector>> field;
        vector<vector<Sector>> FoggedField;
        int *size;
        bool DD=0;
    public:
        Map() = default;
        void Fill_Map(const int* cords);
        void LoadMapLine(string line ,bool which, int index);
        std::vector<std::vector<Sector>> Show();
        std::vector<std::vector<Sector>> ShowFogged();
        bool CheckShip(ShipInfo boat, int length);
        void ShipIsNoMore(vector<int> info);
        bool PlaceShip(Ship ship, ShipInfo info);
        bool SectorChecker(int *cords);
        bool Hit(int *coords, Manager* manage);
        void ActivateDD();
        Map& operator=(const Map& source);
        Map& operator=(Map&& moved);
        int* getSize();
        string getMapLine(int index, bool which);
        Map(const Map& source) : size(source.size), field(source.field){}
        Map(Map&& moved) noexcept: size(moved.size), field(moved.field){}
        ~Map(); 
};
#endif