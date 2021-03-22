#include "stdafx.h"
#include "WaveChoreographer.h"


#include "PlayerInput.h"
#include "TowerDefenseArena.h"

WaveChoreographer::WaveChoreographer(TowerDefenseArena* arena): arena{arena}
{
    enemySpawners.push_back(new EnemySpawner<EnemyBoss>(0));
    enemySpawners.push_back(new EnemySpawner<EnemySoldier>(1));
    enemySpawners.push_back(new EnemySpawner<EnemyHeavy>(1));
    enemySpawners.push_back(new EnemySpawner<EnemyScout>(1));
    enemySpawners.push_back(new EnemySpawner<EnemyCorner>(3));

    lastWaveFinishTimestamp = arena->GetTimeElapsed();
}

void WaveChoreographer::Update()
{
    if (!isWaveInProgress && (arena->GetTimeElapsed() > lastWaveFinishTimestamp + waveDelay
        || arena->GetPlayerInput()->GetKeyPressedThisFrame(VK_SPACE)))
    {
        GenerateNextWave();
    }

    if (isWaveInProgress)
    {
        // Haven't spawned all the enemies required
        if (spawnedEnemies < totalEnemies)
        {
            // Check to start the next burst
            if (enemiesLeftThisBurst == 0 && arena->GetTimeElapsed() > burstFinishTimestamp + GetBurstDelay())
            {
                enemiesLeftThisBurst = GetEnemiesCountThisBurst();
                burstFinishTimestamp = arena->GetTimeElapsed();
            }

            // There are enemies to spawn
            if (enemiesLeftThisBurst > 0)
            {
                // Check if can spawn yet
                if (arena->GetTimeElapsed() > lastSpawnTimestamp + GetBurstSpawnDelay())
                {
                    std::vector<IEnemySpawner*> candidates;

                    for (int i = 0; i < enemySpawners.size(); i++)
                    {
                        if (enemySpawners[i]->CanSpawn())
                        {
                            candidates.push_back(enemySpawners[i]);
                        }
                    }

                    std::random_shuffle(candidates.begin(), candidates.end());

                    if (candidates.size() == 0)
                    {
                        // Something went wrong, just try to reset this.
                        isWaveInProgress = false;
                        lastWaveFinishTimestamp = arena->GetTimeElapsed();
                        for (int i = 0; i < enemySpawners.size(); i++)
                        {
                            enemySpawners[i]->SetEnemyCount(0);
                        }
                        return;
                    }

                    IEnemy* newEnemy = candidates[0]->SpawnEnemy(
                        (*arena->GetPath())[0], arena->GetWorldRoot(), arena->GetPath(),
                        min(arena->tileSizeX, arena->tileSizeZ), GetStrengthMultiplier());
                    arena->AddEnemy(newEnemy);
                    spawnedEnemies++;

                    lastSpawnTimestamp = arena->GetTimeElapsed();
                }
            }
        }
        else
        {
            if (arena->GetEnemyCount() == 0)
            {
                isWaveInProgress = false;
                lastWaveFinishTimestamp = arena->GetTimeElapsed();
            }
        }
    }
}

void WaveChoreographer::GenerateNextWave()
{
    wave++;
    isWaveInProgress = true;
    int waveStrengthBudget = GetWaveStrength();
    totalEnemies = 0;
    spawnedEnemies = 0;

    if (wave % 5 == 0)
    {
        enemySpawners[BOSS_SPAWNER_ID]->SetEnemyCount(wave / 5);
        totalEnemies = wave / 5;
    }
    else
    {
        while (waveStrengthBudget > 0)
        {
            const int randomSpawnerId = rand() % (enemySpawners.size() - 1) + 1;
            waveStrengthBudget -= enemySpawners[randomSpawnerId]->GetEnemySpawnStrength();
            enemySpawners[randomSpawnerId]->AddEnemyCount(1);
            totalEnemies++;
        }
    }
}
