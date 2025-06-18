#include <iostream>
#define NOMINMAX
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <list>
#include <chrono>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Headers
#include "player.h"
#include "enemy.h"
#include "scoreboard.h"
#include "func.h"

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
	window.setFramerateLimit(60);

	enum Place
	{
		MENU,
		BATTLE
	};

	Place place = MENU;

	Player player("Player", 100, 100, 100);
	Enemy goblin("Goblin", 5);

	// Loading assets
	sf::SoundBuffer soundBuffer;
	if (!soundBuffer.loadFromFile("audio/sound.wav"))
		return EXIT_FAILURE;
	sf::Sound sound(soundBuffer);

	sf::Texture menuBgTexture;
	if (!menuBgTexture.loadFromFile("assets/menuBg.png"))
	{
		return EXIT_FAILURE;
	}
	sf::Sprite menuBackground(menuBgTexture);

	sf::Texture battleBg;
	if (!battleBg.loadFromFile("assets/BattleBg.jpg"))
	{
		return EXIT_FAILURE;
	}
	sf::Sprite backgroundSprite(battleBg);

	sf::Texture papyrus;
	if (!papyrus.loadFromFile("assets/Papyrus.png"))
	{
		return EXIT_FAILURE;
	}
	sf::Sprite foregroundSprite(papyrus);

	foregroundSprite.setPosition(910, 130);
	foregroundSprite.setScale(2.4f, 2.4f);

	sf::FloatRect spriteBounds = foregroundSprite.getGlobalBounds();

	struct Menu
	{
		std::string writing;
		sf::Text text;
	};

	std::vector<Menu> menu;
	int menuIndex = 0;

	// Displayed text
	sf::Font font;
	font.loadFromFile("./fonts/ITCBLKAD.TTF");

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(100);
	text.setPosition(100, 200);
	text.setFillColor(sf::Color::White);

	Menu menu1{ "Play", text };
	menu1.text.setString(menu1.writing);
	menu1.text.setPosition(215, 300);
	menu.push_back(menu1);

	Menu menu2{ "Quit", text };
	menu2.text.setString(menu2.writing);
	menu2.text.setPosition(880, 300);
	menu.push_back(menu2);

	sf::Text text2("", font, 45);
	text2.setFillColor(sf::Color::Black);

	sf::Text youType("", font, 45);
	youType.setFillColor(sf::Color::Black);

	sf::Text playerHealth;
	playerHealth.setFont(font);
	playerHealth.setCharacterSize(40);
	playerHealth.setPosition(955, 418.f);
	playerHealth.setFillColor(sf::Color::Black);
	playerHealth.setString("Player Health: " + std::to_string(player.getPlayerHealth()));

	sf::Text goblinHealth;
	goblinHealth.setFont(font);
	goblinHealth.setCharacterSize(40);
	goblinHealth.setPosition(955, 468.f);
	goblinHealth.setFillColor(sf::Color::Red);
	goblinHealth.setString("Goblin Health: " + std::to_string(goblin.getEnemyHealth()));

	sf::Text instructionText("Type the word:", font, 45);
	instructionText.setFillColor(sf::Color::Black);
	sf::FloatRect instrBounds = instructionText.getLocalBounds();
	instructionText.setOrigin(instrBounds.left + instrBounds.width / 2.f, instrBounds.top + instrBounds.height / 2.f);
	instructionText.setPosition(spriteBounds.left + spriteBounds.width / 2.f, spriteBounds.top + 95.f);

	std::string youTypeStr;
	std::list<std::string>::iterator currentWord = loadedWords.begin();

	if (currentWord != loadedWords.end()) {
		text2.setString(*currentWord);
		centerTextWithOffset(text2, spriteBounds, -100.f);
	}

	Scoreboard score;
	int pointCatcher = 0;
	score.alterPoints(pointCatcher);

	bool typingEnabled = true;
	bool keyHeld = false;
	bool professorFlag = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}

			if (place == MENU)
				// Menu configs
			{
				if (event.type == sf::Event::KeyPressed)
				{
					if (!keyHeld)
					{
						if (event.key.code == sf::Keyboard::Right && menuIndex < menu.size() - 1)
						{
							menuIndex++;
							keyHeld = true;
						}
						else if (event.key.code == sf::Keyboard::Left && menuIndex > 0)
						{
							menuIndex--;
							keyHeld = true;
						}
					}
				}
				else if (event.type == sf::Event::KeyReleased &&
					(event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right))
				{
					keyHeld = false;
				}
			}

			if (typingEnabled && event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 128)
				{
					youTypeStr += static_cast<char>(event.text.unicode);
					youType.setString(youTypeStr);
					centerTextWithOffset(youType, spriteBounds, -5.f);
				}
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
			{
				if (place == MENU)
				{
					if (menuIndex == 0)
					{
						place = BATTLE;
						// Music
						sound.setLoop(true);
						//sound.play();
					}
					else if (menuIndex == 1)
					{
						window.close();
					}
				}
				else if (place == BATTLE)
				{
					youTypeStr = trim(youTypeStr);

					if (currentWord != loadedWords.end() && youTypeStr == *currentWord)
					{
						// Deal damage
						pointCatcher = 1 * player.getPlayerDmg();
						goblin.getDmg(pointCatcher);
						goblin.updateTexture();
						score.alterPoints(pointCatcher);
						text2.setFillColor(sf::Color::Black);
						++currentWord;

						if (currentWord != loadedWords.end())
						{
							text2.setString(*currentWord);
							centerTextWithOffset(text2, spriteBounds, -100.f);
							youTypeStr.clear();
							youType.setString("");
						}
					}
					else if (youTypeStr == "matrix")
					{
						player.cheatTexture();
						professorFlag = true;
						youTypeStr.clear();
						youType.setString("");
					}
					else
					{
						// Take damage
						pointCatcher = 1;
						player.getDmg(pointCatcher);
						if (!professorFlag)
						{
							player.updateTexture();
						}
						score.alterPoints(pointCatcher);
						text2.setFillColor(sf::Color::Red);
						youTypeStr.clear();
						youType.setString("");
						centerTextWithOffset(youType, spriteBounds, -5.f);
					}
				}
			}
		}
	
		switch (place)
		{
		case MENU:
			window.clear();
			menuBackground.setTexture(menuBgTexture);
			window.draw(menuBackground);
			for (int i = 0; i < menu.size(); i++) {
				if (i == menuIndex) {
					menu[i].text.setFillColor(sf::Color::Red);
				}
				else {
					menu[i].text.setFillColor(sf::Color::White);
				}
			}
			for (const auto& m : menu) {
				window.draw(m.text);
			}

			break;
		case BATTLE:
			window.clear();
			window.draw(backgroundSprite);
			player.draw(&window);
			goblin.draw(&window);

			window.draw(foregroundSprite);
			window.draw(text2);
			window.draw(instructionText);
			window.draw(youType);

			playerHealth.setString("Player Health: " + std::to_string(player.getPlayerHealth()));
			window.draw(playerHealth);
			goblinHealth.setString("Goblin Health: " + std::to_string(goblin.getEnemyHealth()));
			window.draw(goblinHealth);

			if (goblin.getEnemyHealth() <= 0)
			{
				text2.setString("Victory");
				centerTextWithOffset(text2, spriteBounds, -100.f);
				youTypeStr.clear();
				youType.setString("");
				instructionText.setString("");
				typingEnabled = false;
			}
			if (player.getPlayerHealth() <= 0)
			{
				text2.setString("Defeat");
				centerTextWithOffset(text2, spriteBounds, -100.f);
				youTypeStr.clear();
				youType.setString("");
				instructionText.setString("");
				typingEnabled = false;
				break;
		default:
			break;
			}
		}

		window.display();
	};

	return 0;
}