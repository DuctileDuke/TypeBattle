#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string& nam, int heal)
    : GameObj(nam, heal)
{
    if (!texture.loadFromFile("assets/goblin.png"))
    {
        throw std::invalid_argument("Failed to load enemy texture");
    }

    if (!texture2.loadFromFile("assets/goblinDead.png"))
    {
        throw std::invalid_argument("Failed to load enemy texture");
    }

    sprite.setTexture(texture);
    sprite.setScale(.8, .8);
    sprite.setPosition(600, 340);
}

void Enemy::getDmg(int dmg)
{
    if (dmg < 0)
    {
        throw std::invalid_argument("Negative damage!");
    }
    health -= dmg;
    if (health < 0) {
        health = 0;
    }
}

void Enemy::draw(sf::RenderWindow* window)
{
    window->draw(sprite);
}

void Enemy::updateTexture()
{
    if (health <= 0) {
        sprite.setTexture(texture2);
    }
    else {
        sprite.setTexture(texture);
    }
}
