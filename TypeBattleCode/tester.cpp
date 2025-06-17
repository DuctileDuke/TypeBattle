#define NOMINMAX
#include <iostream>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>

#include "Player.h"
#include "Enemy.h"
#include "Button.h"

int main()
{

    Player player("Player", 100, gameWidth, gameHeight);
    Enemy goblin("Goblin", 30);

    enum Place
    {
        GOBLIN
    };

    Place place = GOBLIN;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Type Battle", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("bgnew.jpg"))
    {
        return EXIT_FAILURE;
    }

    sf::Texture inarenaTexture;
    if (!inarenaTexture.loadFromFile("battle.jpg"))
    {
        return EXIT_FAILURE;
    }

    sf::Sprite background(bgTexture);

    sf::Vector2u textureSize = bgTexture.getSize();

    float scaleX = static_cast<float>(gameWidth) / textureSize.x;
    float scaleY = static_cast<float>(gameHeight) / textureSize.y;

    background.setScale(scaleX, scaleY);

    sf::Font font;
    if (!font.loadFromFile("ITCBLKAD.TTF"))
        return EXIT_FAILURE;

    sf::Texture parchmentTexture;
    if (!parchmentTexture.loadFromFile("parchment2.png"))
    {
        return EXIT_FAILURE;
    }
    sf::Sprite parchment(parchmentTexture);
    parchment.scale(3.80, 3.80);
    parchment.setPosition(gameWidth * 0.68, gameHeight / 2 - parchment.getGlobalBounds().height / 2);

    sf::Text playerHealth;
    playerHealth.setFont(font);
    playerHealth.setCharacterSize(40);
    playerHealth.setPosition(gameWidth * 0.75, 250.f);
    playerHealth.setFillColor(sf::Color::Black);
    playerHealth.setString("Player Health: " + std::to_string(player.getPlayerHealth()));

    sf::Text goblinHealth;
    goblinHealth.setFont(font);
    goblinHealth.setCharacterSize(40);
    goblinHealth.setPosition(gameWidth * 0.75, 300.f);
    goblinHealth.setFillColor(sf::Color::Red);
    goblinHealth.setString("Goblin Health: " + std::to_string(goblin.getEnemyHealth()));

    sf::Text victoryText;
    victoryText.setFont(font);
    victoryText.setString("VICTORY!");
    victoryText.setCharacterSize(40);
    victoryText.setFillColor(sf::Color::Red);
    victoryText.setPosition(gameWidth * 0.73, gameHeight * 0.7);

    sf::Text defeatText;
    defeatText.setFont(font);
    defeatText.setString("DEFEAT!");
    defeatText.setCharacterSize(40);
    defeatText.setFillColor(sf::Color::Red);
    defeatText.setPosition(gameWidth * 0.75, gameHeight * 0.7);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                if (threadKontratak.joinable())
                {
                    threadKontratak.join();
                }
                window.close();
            }
if (place == GOBLIN)
            {
                player.setPosX(0.05, desktopMode);
                player.setPosY(0.62, desktopMode);
                player.setSize(0.8, 0.8);
                goblin.setPosX(0.48);
                goblin.setPosY(0.75);
                goblin.setSize(0.8, 0.8);

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                        goblin.getDmg(player.getPlayerDmg());
                }
            }
        }

        switch (place)
        {

        case GOBLIN:
            resetWindow(&window, background);
            goblin.draw(&window, desktopMode);
            player.draw(&window);
            window.draw(parchment);
            playerHealth.setString("Player Health: " + std::to_string(player.getPlayerHealth()));
            window.draw(playerHealth);
            goblinHealth.setString("Goblin Health: " + std::to_string(goblin.getEnemyHealth()));
            window.draw(goblinHealth);
            button.draw(&window);
            if (goblin.getEnemyHealth() <= 0)
            {
                window.draw(victoryText);
            }
            else if (player.getPlayerHealth() <= 0)
            {
                window.draw(defeatText);
            }
            break;
        default:
            break;
        }
        window.display();
    }
    return EXIT_SUCCESS;
}