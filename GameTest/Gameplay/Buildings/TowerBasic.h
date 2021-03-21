#pragma once
#include "IBuilding.h"
#include "IShootingTower.h"
#include "../../Drawables/DrawableRhombus.h"

class TowerBasic : public IShootingTower
{
public:
    TowerBasic() : TowerBasic(Float3(1, 0, 0))
    {
    }

    TowerBasic(Float3 baseColor)
        : IShootingTower(baseColor)
    {
        gemStone = new DrawableRhombus(Float3(), this, 1);
    }

    ~TowerBasic()
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
        gemStone->RotateBy(0, deltaTime, 0);
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        gemStone->SetScale(renderScale / 2.0f);
        gemStone->MoveToLocal(Float3(0, renderScale / 2.0f, 0));
    }

    std::string GetName() override
    {
        return "Basic Tower";
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
