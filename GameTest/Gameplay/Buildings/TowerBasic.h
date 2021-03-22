#pragma once
#include "IBuilding.h"
#include "IShootingTower.h"
#include "../../Drawables/DrawableRhombus.h"

class TowerBasic : public IShootingTower
{
public:
    TowerBasic() : TowerBasic(Float3(1, 0, 0), 1.f)
    {
    }

    TowerBasic(Float3 baseColor, float shotCooldown)
        : IShootingTower(baseColor, shotCooldown)
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
        RenderShotTrail(shotLocation, renderer, camera, baseColor,
                        Float3(0, GetUpgradeLevelScale() * renderScale / 3.0f));
        gemStone->Render(renderer, camera, baseColor);
    }

    void Update(float deltaTime) override
    {
        IShootingTower::Update(deltaTime);

        SetScale(GetUpgradeLevelScale());
        gemStone->RotateBy(0, deltaTime, 0);
    }

    boolean TryShoot() override
    {
        if (!target)
        {
            target = AcquireTarget();
        }
        else
        {
            if (tilesInRange.count(target->GetCurrentTile()) == 0)
            {
                target = nullptr;
            }
            else
            {
                shotLocation = target->GetWorldPosition();
                target->Damage(GetDamage());
                return true;
            }
        }

        return false;
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        gemStone->SetScale(renderScale / 2.0f);
        gemStone->MoveToLocal(Float3(0, renderScale / 2.0f, 0));
    }

    std::string GetName() override
    {
        return "Defense Tower";
    }

    std::vector<std::string> GetDescription() override
    {
        return {
            "Primary defense tower.",
            "Nothing flashy but does the job.",
            "Damage: " + std::to_string(static_cast<int>(GetDamage())) + (IsUpgradeable() ? " (+4 next level)" : ""),
            "Range: " + std::to_string(static_cast<int>(GetRange())) + (IsUpgradeable() ? " (+1 next level)" : ""),
            "Fire rate: Medium"
        };
    }

    int GetCost() override
    {
        return 10 + 5 * GetLevel();
    }

    int GetRange() override
    {
        return 1 + GetLevel();
    }

    float GetDamage() override
    {
        return 5.0f + 4.0f * GetLevel();
    }

    float GetUpgradeLevelScale() const override
    {
        return 0.5f + GetLevel() * 0.25f;
    }

    int GetMaxLevel() const override
    {
        return 2;
    }

protected:
    IDrawable* gemStone;
};
