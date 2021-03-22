#pragma once
#include "IBuilding.h"
#include "../TowerDefenseArena.h"

class IAreaEffectTower : public IBuilding
{
public:
    IAreaEffectTower() : IAreaEffectTower(Float3(0, 0, 1))
    {
    }

    IAreaEffectTower(Float3 baseColor)
        : IBuilding(baseColor)
    {
    }

    ~IAreaEffectTower()
    {
    }

    void OnPlaced() override
    {
        IBuilding::OnPlaced();
        ApplyEffectToAllTiles();
    }

    void OnWillUpgrade() override
    {
        RemoveEffectFromAllTiles();
    }

    void OnUpgraded() override
    {
        IBuilding::OnUpgraded();
        ApplyEffectToAllTiles();
    }

    virtual void ApplyEffectToTile(ArenaTile* tile);

    virtual void RemoveEffectFromTile(ArenaTile* tile);

    void ApplyEffectToAllTiles()
    {
        for (std::unordered_set<ArenaTile*>::iterator itr = tilesInRange.begin(); itr != tilesInRange.end(); ++itr)
        {
            ArenaTile* traversalTile = *itr;
            if (traversalTile)
            {
                ApplyEffectToTile(traversalTile);
            }
        }
    }

    void RemoveEffectFromAllTiles()
    {
        for (std::unordered_set<ArenaTile*>::iterator itr = tilesInRange.begin(); itr != tilesInRange.end(); ++itr)
        {
            ArenaTile* traversalTile = *itr;
            if (traversalTile)
            {
                RemoveEffectFromTile(traversalTile);
            }
        }
    }
};
