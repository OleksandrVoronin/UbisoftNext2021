#pragma once
#include "../ArenaTile.h"
#include "../../Math/Float3.h"
#include "../../Utils/Camera.h"
#include "../../Utils/LineRenderer.h"
#include "../../Utils/Transform.h"


class TowerDefenseArena;
double GetCounter();

class IBuilding : public Transform
{
public:
    IBuilding(Float3 baseColor) : baseColor{baseColor}
    {
    }

    IBuilding() : baseColor{0, 0, 0}
    {
    }

    virtual ~IBuilding() = default;

    // Core updates
    virtual void Update(float deltaTime) = 0;
    virtual void Render(LineRenderer* renderer, Camera* camera);


    // Lifetime events to redefine in children if needed.
    virtual void OnPlaced()
    {
        UpdateTilesInRange();
    }

    virtual void OnWillUpgrade()
    {
    }

    virtual void OnUpgraded()
    {
        UpdateTilesInRange();
    }


    // Renders a covered-range visual around the building
    virtual void RenderRange(LineRenderer* renderer, Camera* camera);

    virtual void SetRenderScale(float renderScale);

    void SetTile(ArenaTile* tile);
    void SetArena(TowerDefenseArena* tile);

    boolean IsUpgradeable() const;

    int GetLevel() const;

    virtual int GetMaxLevel() const
    {
        return 0;
    }

    virtual int GetSellPrice()
    {
        return GetCost() / 2;
    }

    virtual void Upgrade()
    {
        OnWillUpgrade();

        if (level < GetMaxLevel())
        {
            level++;
        }

        OnUpgraded();
    }

    virtual int GetCost() = 0;

    virtual std::string GetName() = 0;

    virtual std::vector<std::string> GetDescription() = 0;

    virtual int GetRange() = 0;

    virtual float GetDamage() = 0;

    virtual float GetUpgradeLevelScale() const;

protected:
    TowerDefenseArena* arena = nullptr;
    ArenaTile* tile = nullptr;
    std::unordered_set<ArenaTile*> tilesInRange;

    Float3 baseColor;
    float renderScale = 1.0f;
    int level = 0;

    void UpdateTilesInRange();
    void RenderBuildingBase(LineRenderer* renderer, Camera* camera) const;
};
