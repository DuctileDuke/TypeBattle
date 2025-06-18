#include "scoreboard.h"
#include <iostream>

using namespace std::chrono;

Scoreboard::Scoreboard() {
    startTyping = steady_clock::now();
    score = 0;
}

Scoreboard::~Scoreboard() {
    auto endTyping = steady_clock::now();
    duration<double> elapsed = endTyping - startTyping;
    std::cout << "Time: " << elapsed.count() << "s" << std::endl;
    std::cout << "Final Score: " << score << std::endl;
}

void Scoreboard::alterPoints(int points) {
    score += points;
}

float Scoreboard::share() {
    return score;
}