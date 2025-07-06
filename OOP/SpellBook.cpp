#include "SpellBook.h"
void SpellBook::getSpells(Map* newMap, Manager* newManager){
    queue.push_back(std::make_unique<DoubleDamage>());
    queue.push_back(std::make_unique<Scanner>());
    queue.push_back(std::make_unique<Barrage>());
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(queue.begin(), queue.end(), g);
    manage = newManager;
    field = newMap;
}
void SpellBook::addSpell(){
    srand(time(0));
    switch (rand()%3)
    {
    case 0:
        queue.insert(queue.begin(), std::make_unique<DoubleDamage>());
        break;
    case 1:
        queue.insert(queue.begin(), std::make_unique<Scanner>());
        break;
    case 2:
        queue.insert(queue.begin(), std::make_unique<Barrage>());
        break;
    default:
        break;
    }
}
std::string SpellBook::AllSpellsInStr(){
    std::string result;
    for(int i=0; i<queue.size(); i++){result.append(queue[i]->saySpell());}
    return result;
}
void SpellBook::useSpell(tuple<int, int> cords){
    int tmp[2];
    tmp[0] = get<0>(cords);
    tmp[1] = get<1>(cords);
    try{
    if(queue.size()==0){throw NoSpells();}
    if(queue.back()->cast(field, manage, tmp)){
        addSpell();
    }
    queue.pop_back();
    }
    catch(const NoSpells& e){
        std::cout<<e.what()<<std::endl;
    }
}
void SpellBook::nextSpell(){
    std::cout<<queue.back()->saySpell();
}
int SpellBook::AmountoSpells(){
    return queue.size();
}
void SpellBook::LoadSpell(std::string name){
    if(name == "DoubleDamage"){
        queue.push_back(std::make_unique<DoubleDamage>());
    }
    if(name == "Scanner"){
        queue.push_back(std::make_unique<Scanner>());
    }
    if(name == "Barrage"){
        queue.push_back(std::make_unique<Barrage>());
    }
}
void SpellBook::update(Map* newMap, Manager* newManager){
    manage = newManager;
    field = newMap;
}