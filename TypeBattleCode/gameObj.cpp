#include "GameObj.h"

GameObj::GameObj(const std::string& nam, int heal) : name(nam), health(heal) {}

int GameObj::getHealth() const
{
    return health;
}

std::string GameObj::getName() const
{
    return name;
}