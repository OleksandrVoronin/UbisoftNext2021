#pragma once
#include "IAreaEffectTower.h"
#include "IBuilding.h"
#include "../../Drawables/DrawableSnowflake.h"

class TowerFrost : public IAreaEffectTower
{
public:
    TowerFrost() : TowerFrost(Float3(0, 0, 1))
    {
    }

    TowerFrost(Float3 baseColor)
        : IAreaEffectTower(baseColor)
    {
        snowflake = new DrawableSnowflake(Float3(), this, 1);
    }

    ~TowerFrost()
    {
        RemoveEffectFromAllTiles();
        delete snowflake;
    }

    void ApplyEffectToTile(ArenaTile* tile) override
    {
        tile->SetSpeedMultiplier(tile->GetSpeedMultiplier() * GetSpeedMultiplier());
    }

    void RemoveEffectFromTile(ArenaTile* tile) override
    {
        tile->SetSpeedMultiplier(tile->GetSpeedMultiplier() * (1.0f / GetSpeedMultiplier()));
    }

    void Render(LineRenderer* renderer, Camera* camera) override
    {
        IBuilding::Render(renderer, camera);
        snowflake->Render(renderer, camera, baseColor);
    }

    void Update(float deltaTime) override
    {
        SetScale(GetUpgradeLevelScale());
        snowflake->RotateBy(deltaTime, deltaTime, deltaTime);
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        snowflake->SetScale(renderScale / 2.0f);
        snowflake->MoveToLocal(Float3(0, renderScale / 2.0f, 0));
    }

    std::string GetName() override
    {
        return "Sigil of Frost";
    }

    std::vector<std::string> GetDescription() override
    {
        return {
            "Freezing aura from this tower slows enemies",
            "in range.",
            "Damage: 0",
            "Range: " + std::to_string(static_cast<int>(GetRange())) + (IsUpgradeable() ? " (+1 next level)" : ""),
            "Slows from multiple towers stack."
        };
    }

    float GetSpeedMultiplier() const
    {
        return 0.75f;
    }

    int GetCost() override
    {
        return 20 + 30 * GetLevel();
    }

    int GetRange() override
    {
        return 1 + GetLevel();
    }

    float GetDamage() override
    {
        return 0;
    }

    float GetUpgradeLevelScale() const override
    {
        return 0.6f + GetLevel() * 0.4f;
    }

    int GetMaxLevel() const override
    {
        return 1;
    }

private:
    DrawableSnowflake* snowflake;
};
