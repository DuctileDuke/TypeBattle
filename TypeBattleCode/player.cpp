#include "player.h"
#include <iostream>
#include <stdexcept>

Player::Player(const std::string& nam, int heal, int maxX, int maxY)
    : GameObj(nam, heal), maxX(maxX), maxY(maxY)
{
    if (!texture.loadFromFile("assets/player.png"))
    {
        throw std::invalid_argument("Failed to load player texture");
    }

    if (!texture2.loadFromFile("assets/playerDead.png"))
    {
        throw std::invalid_argument("Failed to load enemy texture");
    }

    if (!texture3.loadFromFile("assets/professorA.png"))
    {
        throw std::invalid_argument("Failed to load enemy texture");
    }

    sprite.setTexture(texture);
    sprite.setScale(.8, .8);
    sprite.setPosition(100, 350);
}

void Player::getDmg(int dmg)
{
    if (dmg < 0)
    {
        throw std::invalid_argument("Negative damege!");
    }
    health -= dmg;
    if (health < 0)
    {
        health = 0;
    }
}

void Player::draw(sf::RenderWindow* window)
{
    window->draw(sprite);
}

void Player::updateTexture()
{
    if (health <= 0) {
        sprite.setTexture(texture2);
        sprite.setPosition(100, 450);
    }
    else {
        sprite.setTexture(texture);
    }
}

void Player::cheatTexture()
{
    sprite.setTexture(texture3, true);
    sprite.setScale(.3, .3);
    sprite.setPosition(100, 220);
    health = 100;
    dmg = 100;
}