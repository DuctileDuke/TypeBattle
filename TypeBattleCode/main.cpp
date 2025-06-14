#include <iostream>
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

using namespace std::chrono;

static int RNG() {
    return std::rand() % 50 + 1;
}

std::string trim(std::string smth) {
    smth.erase(std::remove(smth.begin(), smth.end(), '\r'), smth.end());
    return smth;
}

class Scoreboard {
public:
    Scoreboard() {
        StartTyping = steady_clock::now();
        Score = 0;
    }

    ~Scoreboard() {
        auto EndTyping = steady_clock::now();
        duration<double> elapsed = EndTyping - StartTyping;
        std::cout << "Time: " << elapsed.count() << "s" << std::endl;
        std::cout << "Final Score: " << Score << std::endl;
    }

    void AlterPoints(int points) {
        Score += points;
    }

    float Share() {
        return Score;
    }

private:
    time_point<steady_clock> StartTyping;
    float Score;
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Adding random IDs
    std::list<int> nums;
    for (int i = 0; i < 5; ++i) {
        nums.push_back(RNG());
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

    std::list<std::string> LoadedWords;

    for (const int& RNumber : nums) {

        // Loading words from the database based on random IDs
        std::string query = "SELECT * FROM WordSource WHERE Id = " + std::to_string(RNumber);

        SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);
        retCode = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);

        SQLCHAR columnData[256];
        if (SQLFetch(stmtHandle) == SQL_SUCCESS) {
            SQLGetData(stmtHandle, 2, SQL_C_CHAR, columnData, sizeof(columnData), NULL);
            std::string TempWord(reinterpret_cast<const char*>(columnData));
            LoadedWords.push_back(trim(TempWord));
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
    }

    // Close database connection
    SQLDisconnect(connHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, connHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, envHandle);

    Scoreboard scoreboard;
    std::string YouType;
    auto currentWord = LoadedWords.begin();

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    // Testing font
    sf::Font font;
    font.loadFromFile("./fonts/ITCBLKAD.TTF");

    sf::Text text;
    text.setFont(font);
    text.setString("DZIALA !!!!!!");
    text.setCharacterSize(100);
    text.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }

    while (true) {

        // Main game loop
        if (currentWord == LoadedWords.end()) {
            std::cout << "Stats: " << std::endl;
            break;
        }

        std::cout << "Type the word: " << *currentWord << std::endl;
        std::cout << "> ";
        std::getline(std::cin, YouType);
        YouType = trim(YouType);

        if (YouType == *currentWord) {
            std::cout << "+10" << std::endl;
            scoreboard.AlterPoints(10);
            ++currentWord;
        }
        else {
            std::cout << "-5" << std::endl;
            scoreboard.AlterPoints(-5);
        }
    }

    return 0;
}