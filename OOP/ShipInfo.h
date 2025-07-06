#ifndef SHIPINFP_H
#define SHIPINFO_H
class ShipInfo{
    public:
    int x;
    int y;
    char ori;
    ShipInfo(int hor, int ver);
    void Set_ori(char dir);
    ~ShipInfo();
};
#endif //SHIPINFO_H