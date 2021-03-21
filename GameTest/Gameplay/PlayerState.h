#pragma once

class PlayerState
{
public:

    void DeltaCredits(int creditsDelta)
    {
        credits += creditsDelta;
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

private:
    int credits = 500;
    int lives = 5;
    int score = 0;
};
