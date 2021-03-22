#pragma once
#include "Enemies/IEnemy.h"

class IEnemySpawner
{
public:
    IEnemySpawner(int enemySpawnStrength) : enemySpawnStrength{enemySpawnStrength}
    {
    }

    virtual IEnemy* SpawnEnemy(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
                               float renderScale, float strengthMultiplier) = 0;

    virtual ~IEnemySpawner() = default;

    void AddEnemyCount(int delta)
    {
        this->enemyCount += delta;
    }

    void SetEnemyCount(int enemyCount)
    {
        this->enemyCount = enemyCount;
    }

    int GetEnemySpawnStrength() const
    {
        return enemySpawnStrength;
    }

    boolean CanSpawn() const
    {
        return enemyCount > 0;
    }

protected:
    int enemyCount = 0;
    int enemySpawnStrength = 0;
};

template <typename T = IEnemy>
class EnemySpawner : public IEnemySpawner
{
public:
    EnemySpawner(int enemySpawnStrength)
        : IEnemySpawner(enemySpawnStrength)
    {
    }

    T* SpawnEnemy(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
                  float renderScale, float strengthMultiplier) override
    {
        enemyCount--;
        return new T(position, worldRoot, path, renderScale, strengthMultiplier);
    }
};
