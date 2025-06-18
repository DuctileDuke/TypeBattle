#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "gameObj.h"

class Player : public GameObj
{
private:
    sf::Sprite sprite;
    sf::Texture texture;

    int maxX;
    int maxY;

    int speed = 1;

    int health = 10;
    int dmg = 2;

public:
    Player(const std::string& nam, int heal, int maxX, int maxY);

    void getDmg(int dmg) override;

    void draw(sf::RenderWindow* window);

    int getPlayerHealth()
    {
        return health;
    }

};

#endif