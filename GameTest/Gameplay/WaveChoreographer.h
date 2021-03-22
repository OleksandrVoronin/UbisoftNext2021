#pragma once
#include <algorithm>
#include <stdlib.h>


#include "EnemySpawner.h"
#include "TowerDefenseArena.h"
#include "Enemies/EnemyBoss.h"
#include "Enemies/EnemyCorner.h"
#include "Enemies/EnemyHeavy.h"
#include "Enemies/EnemyScout.h"
#include "Enemies/EnemySoldier.h"

class TowerDefenseArena;

class WaveChoreographer
{
public:
    WaveChoreographer(TowerDefenseArena* arena);

    void Update();

    boolean GetIsWaveInProgress() const
    {
        return isWaveInProgress;
    }

    float GetTimeUntilNextWave() const
    {
        return lastWaveFinishTimestamp + waveDelay - arena->GetTimeElapsed();
    }

    int GetWaveNumber() const
    {
        return wave;
    }

private:
    int wave = 0;

    TowerDefenseArena* arena;
    std::vector<IEnemySpawner*> enemySpawners;
    const int BOSS_SPAWNER_ID = 0;
    const int waveDelay = 20;
    boolean isWaveInProgress = false;

    int enemiesLeftThisBurst = 0;
    float burstFinishTimestamp = std::numeric_limits<float>::lowest();
    float lastSpawnTimestamp = std::numeric_limits<float>::lowest();
    float lastWaveFinishTimestamp = std::numeric_limits<float>::lowest();

    // Total counts of enemies allocated for this wave
    int totalEnemies = 0;
    int spawnedEnemies = 0;

    // Total strength of this level
    int GetWaveStrength() const
    {
        return 20 * wave;
    }


    // Delay between bursts (multiple spawns within a short timeframe).
    float GetBurstDelay() const
    {
        return max(0.5f, 3 - (wave * 0.1f));
    }

    // Enemies to spawn within this burst.
    int GetEnemiesCountThisBurst() const
    {
        return 5 * wave;
    }

    // Delay between spawns within a burst.
    float GetBurstSpawnDelay() const
    {
        return max(0.1f, 1 - (wave * 0.05f));
    }


    // Stat multiplier applied to all new enemies.
    float GetStrengthMultiplier() const
    {
        return 1 + 0.2f * wave;
    }

    void GenerateNextWave();
};
