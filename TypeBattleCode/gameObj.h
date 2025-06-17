#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP

#include <string>

class GameObj
{
protected:
    std::string name;
    int health;

public:
    GameObj(const std::string& nam, int heal);
    virtual ~GameObj() = default;

    virtual void getDmg(int dmg) = 0;

    int getHealth() const;
    std::string getName() const;
};

#endif