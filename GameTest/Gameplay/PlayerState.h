#pragma once
#include <dsound.h>
#include <limits>

class PlayerState
{
public:
    void DeltaCredits(int creditsDelta)
    {
        credits += creditsDelta;
    }

    void DeltaLives(int livesDelta)
    {
        lives += livesDelta;
    }

    int GetCredits() const
    {
        return credits;
    }

    int GetLives() const
    {
        return lives;
    }

    int GetScore() const
    {
        return score;
    }

    boolean IsAlive() const
    {
        return lives > 0;
    }


private:
    int credits = 50;
    int lives = 5;
    int score = 0;
};
