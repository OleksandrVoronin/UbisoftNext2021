#pragma once
#include <dsound.h>
#include <unordered_set>


#include "../Math/Float3.h"

class IBuilding;
class IEnemy;

class ArenaTile
{
public:
    ArenaTile(Float3 position, boolean partOfThePath)
        : position{position}, partOfThePath{partOfThePath}
    {
    }

    Float3 GetTileCoordinates() const
    {
        return position;
    }

    boolean ValidForBuildingPlacement() const
    {
        return !GetIsPartOfThePath() && (GetBuilding() == nullptr);
    }

    boolean GetIsPartOfThePath() const
    {
        return partOfThePath;
    }

    std::unordered_set<IEnemy*>* GetEnemiesOnThisTile();

    void AddEnemy(IEnemy* enemy)
    {
        enemiesAtThisTile.insert(enemy);
    }

    void RemoveEnemy(IEnemy* enemy)
    {
        enemiesAtThisTile.erase(enemy);
    }

    IBuilding* GetBuilding() const
    {
        return building;
    }

    void RemoveBuilding()
    {
        building = nullptr;
    }

    void RegisterBuilding(IBuilding* building);

    float GetSpeedMultiplier() const;
    float GetDamageTakenMultiplier() const;

    void SetSpeedMultiplier(float speedMultiplier);
    void SetDamageTakenMultiplier(float damageTakenMultiplier);

private:
    Float3 position;
    boolean partOfThePath = false;
    IBuilding* building = nullptr;

    float speedMultiplier = 1.0f;
    float damageTakenMultiplier = 1.0f;

    std::unordered_set<IEnemy*> enemiesAtThisTile;
};
