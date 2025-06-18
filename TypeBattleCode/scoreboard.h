#include <chrono>

class Scoreboard {
public:
    Scoreboard();
    ~Scoreboard();

    void alterPoints(int points);
    float share();

private:
    std::chrono::time_point<std::chrono::steady_clock> startTyping;
    float score;
};