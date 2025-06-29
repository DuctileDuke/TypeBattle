#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "gameObj.h"

class Player : public GameObj
{
private:
    sf::Sprite sprite;
    sf::Texture texture;
	sf::Texture texture2;
    sf::Texture texture3;

    int health = 3;
    int dmg = 2;

public:
    Player(const std::string& nam, int heal);

    void getDmg(int dmg) override;

    void draw(sf::RenderWindow* window);

    int getPlayerHealth()
    {
        return health;
    }

    int getPlayerDmg()
    {
        return dmg;
    }

    void reset();

    void updateTexture();

    void cheatTexture();

};

#endif