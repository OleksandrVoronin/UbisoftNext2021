#pragma once
#include "IShootingTower.h"
#include "TowerBasic.h"
#include "../../Drawables/DrawableGem.h"

class TowerAOE : public IShootingTower
{
public:
    TowerAOE() : TowerAOE(Float3(1, 0, 1))
    {
    }

    TowerAOE(Float3 baseColor) : IShootingTower(baseColor)
    {
        gemStone = new DrawableGem(Float3(), this, 1);
    }

    ~TowerAOE()
    {
        delete gemStone;
    }

    void Render(LineRenderer* renderer, Camera* camera) override
    {
        IBuilding::Render(renderer, camera);
        gemStone->Render(renderer, camera, baseColor);
    }

    void Update(float deltaTime) override
    {
        SetScale(GetUpgradeLevelScale());
        gemStone->RotateBy(0, deltaTime * 0.5f, 0);
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        gemStone->SetScale(renderScale / 1.8f);
        gemStone->MoveToLocal(Float3(0, renderScale / 6.0f, 0));
    }

    std::string GetName() override
    {
        return "Splash Damage Tower";
    }

    std::vector<std::string> GetDescription() override
    {
        return {
            "Bread and butter of your defense",
            "Damage: " + std::to_string(static_cast<int>(GetDamage())) + (IsUpgradeable() ? " (+4 next level)" : ""),
            "Range: " + std::to_string(GetRange())
        };
    }

    int GetCost() override
    {
        return 10 + 5 * GetLevel();
    }

    int GetRange() override
    {
        return 2;
    }

    float GetDamage() override
    {
        return 10.0f + 4.0f * GetLevel();
    }

    float GetUpgradeLevelScale() const override
    {
        return 0.5f + GetLevel() * 0.25f;;
    }

    int GetMaxLevel() const override
    {
        return 2;
    }

protected:
    IDrawable* gemStone;
};
