#include <iostream>
#define NOMINMAX
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>
#include <algorithm>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "player.h"
#include "enemy.h"

using namespace std::chrono;

static int rng() {
    return std::rand() % 50 + 1;
}

static std::string trim(std::string smth) {
    smth.erase(std::remove(smth.begin(), smth.end(), '\r'), smth.end());
    return smth;
}

class Scoreboard {
public:
    Scoreboard() {
        startTyping = steady_clock::now();
        score = 0;
    }

    ~Scoreboard() {
        auto endTyping = steady_clock::now();
        duration<double> elapsed = endTyping - startTyping;
        std::cout << "Time: " << elapsed.count() << "s" << std::endl;
        std::cout << "Final Score: " << score << std::endl;
    }

    void alterPoints(int points) {
        score += points;
    }

    float share() {
        return score;
    }

private:
    time_point<steady_clock> startTyping;
    float score;
};

void centerTextWithOffset(sf::Text& text, const sf::FloatRect& container, float offsetY) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    text.setPosition(container.left + container.width / 2.f, container.top + container.height / 2.f + offsetY);
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Adding random IDs
    std::list<int> nums;
    for (int i = 0; i < 10; ++i) {
        nums.push_back(rng());
    }

    SQLHANDLE envHandle = nullptr;
    SQLHANDLE connHandle = nullptr;
    SQLHANDLE stmtHandle = nullptr;
    SQLRETURN retCode;

    retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);
    retCode = SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    retCode = SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &connHandle);

    // Database connection
    std::string connStr =
        "Driver={Microsoft Access Driver (*.mdb, *.accdb)};"
        "DBQ=./data/Words.accdb;";

    retCode = SQLDriverConnect(connHandle, NULL,
        (SQLCHAR*)(const unsigned char*)connStr.c_str(),
        SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
    if (!SQL_SUCCEEDED(retCode)) {
        std::cout << "Error - Connection failed." << std::endl;
        return 1;
    }

    std::list<std::string> loadedWords;

    for (const int& rNumber : nums) {

        // Loading words from the database based on random IDs
        std::string query = "SELECT * FROM WordSource WHERE Id = " + std::to_string(rNumber);

        SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);
        retCode = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);

        SQLCHAR columnData[256];
        if (SQLFetch(stmtHandle) == SQL_SUCCESS) {
            SQLGetData(stmtHandle, 2, SQL_C_CHAR, columnData, sizeof(columnData), NULL);
            std::string tempWord(reinterpret_cast<const char*>(columnData));
            loadedWords.push_back(trim(tempWord));
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
    }

    // Close database connection
    SQLDisconnect(connHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, connHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, envHandle);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML window");

    // Loading assets
    sf::Texture battleBg;
    battleBg.loadFromFile("assets/BattleBg.jpg");
    sf::Sprite backgroundSprite(battleBg);

    sf::Texture papyrus;
    papyrus.loadFromFile("assets/Papyrus.png");
    sf::Sprite foregroundSprite(papyrus);
    foregroundSprite.setPosition(910, 130);
    foregroundSprite.setScale(2.4f, 2.4f);

    sf::FloatRect spriteBounds = foregroundSprite.getGlobalBounds();

    sf::Font font;
    font.loadFromFile("./fonts/ITCBLKAD.TTF");

    sf::Text text("", font, 45);
    text.setFillColor(sf::Color::Black);

    sf::Text youType("", font, 45);
    youType.setFillColor(sf::Color::Black);

    sf::Text instructionText("Type the word:", font, 45);
    instructionText.setFillColor(sf::Color::Black);

    sf::FloatRect instrBounds = instructionText.getLocalBounds();
    instructionText.setOrigin(instrBounds.left + instrBounds.width / 2.f, instrBounds.top + instrBounds.height / 2.f);
    instructionText.setPosition(spriteBounds.left + spriteBounds.width / 2.f, spriteBounds.top + 95.f);

    std::string youTypeStr;
    std::list<std::string>::iterator currentWord = loadedWords.begin();

    if (currentWord != loadedWords.end()) {
        text.setString(*currentWord);
        centerTextWithOffset(text, spriteBounds, -100.f);
    }

    Scoreboard score;
    int pointCatcher = 0;
    score.alterPoints(pointCatcher);

    bool typingEnabled = true;

    Player player("Player", 100, 100, 100);
    Enemy goblin("Goblin", 5);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (typingEnabled && event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    youTypeStr += static_cast<char>(event.text.unicode);
                    youType.setString(youTypeStr);
                    centerTextWithOffset(youType, spriteBounds, -5.f);
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                youTypeStr = trim(youTypeStr);

                if (currentWord != loadedWords.end() && youTypeStr == *currentWord) {
                    pointCatcher = 1;
                    goblin.getDmg(pointCatcher);
                    score.alterPoints(pointCatcher);
                    text.setFillColor(sf::Color::Black);
                    ++currentWord;

                    if (currentWord != loadedWords.end()) {
                        text.setString(*currentWord);
                        centerTextWithOffset(text, spriteBounds, -100.f);
                        youTypeStr.clear();
                        youType.setString("");
                    }
                }
                else {
                    pointCatcher = 1;
                    score.alterPoints(pointCatcher);
                    text.setFillColor(sf::Color::Red);
                    youTypeStr.clear();
                    youType.setString("");
                    centerTextWithOffset(youType, spriteBounds, -5.f);
                }
            }
        }

        window.clear();

        window.draw(backgroundSprite);
        player.draw(&window);
        goblin.draw(&window);

        window.draw(foregroundSprite);
        window.draw(text);
        window.draw(instructionText);
        window.draw(youType);

        if (goblin.getEnemyHealth() <= 0)
        {
            text.setString("Victory");
            centerTextWithOffset(text, spriteBounds, -100.f);
            youTypeStr.clear();
            youType.setString("");
            instructionText.setString("");
            typingEnabled = false;
        }

        window.display();
    }
    return 0;
}