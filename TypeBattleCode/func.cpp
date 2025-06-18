#include "func.h"
#include <cstdlib>
#include <algorithm>

int rng() {
    return std::rand() % 50 + 1;
}

std::string trim(std::string smth) {
    smth.erase(std::remove(smth.begin(), smth.end(), '\r'), smth.end());
    return smth;
}

void centerTextWithOffset(sf::Text& text, const sf::FloatRect& container, float offsetY)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    text.setPosition(container.left + container.width / 2.f, container.top + container.height / 2.f + offsetY);
}