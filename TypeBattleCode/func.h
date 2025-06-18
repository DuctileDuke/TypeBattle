#include <string>
#include <SFML/Graphics.hpp>

int rng();

std::string trim(std::string smth);

void centerTextWithOffset(sf::Text& text, const sf::FloatRect& container, float offsetY);