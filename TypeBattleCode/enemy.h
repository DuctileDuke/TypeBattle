#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "gameObj.h"

#include <SFML/Graphics.hpp>

class Enemy : public GameObj
{
private:

    sf::Sprite sprite;
    sf::Texture texture;
	sf::Texture texture2;
    sf::Texture texture3;

    int health = 10;
    int dmg = 1;

public:
    Enemy(const std::string& nam, int heal);

    void getDmg(int dmg) override;

    int getEnemyHealth()
    {
        return health;
    }

    void reset();

    void draw(sf::RenderWindow* window);

    void updateTexture(bool cheat);

};

#endif