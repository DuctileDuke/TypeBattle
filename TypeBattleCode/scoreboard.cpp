#include <windows.h>
#include <sql.h>
#include <ctime>
#include <chrono>
#include <iostream>

using namespace std::chrono;

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
