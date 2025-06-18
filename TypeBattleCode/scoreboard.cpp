#include "scoreboard.h"
#include <iostream>
#include <fstream>

Scoreboard::Scoreboard() {
    startTyping = std::chrono::steady_clock::now();
    score = 0;
}

Scoreboard::~Scoreboard() {
    auto endTyping = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = endTyping - startTyping;
    std::cout << "Time: " << elapsed.count() << "s" << std::endl;
    std::cout << "Final Score: " << score << std::endl;

    std::ofstream file("results.txt", std::ios::app);
    if (file.is_open()) {
        file << "Time: " << elapsed.count() << "s, Score: " << score << "\n";
        file.close();
    }
}

void Scoreboard::alterPoints(int points) {
    score += points;
}

float Scoreboard::share() {
    return score;
}