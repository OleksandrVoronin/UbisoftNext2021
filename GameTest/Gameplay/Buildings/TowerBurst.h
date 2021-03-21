#pragma once
#include "IShootingTower.h"
#include "../../Drawables/DrawablePyramid.h"

class TowerBurst : public IShootingTower
{
public:
    TowerBurst() : TowerBurst(Float3(0.1f, 0.81f, 0.5f))
    {
    }

    TowerBurst(Float3 baseColor)
        : IShootingTower(baseColor)
    {
        rotationArm = new Transform(Float3(), this);
        for (int i = 0; i < 4; i++)
        {
            pyramids[i] = new DrawablePyramid(Float3(), rotationArm, 0.5f);
        }
    }

    ~TowerBurst()
    {
        for (int i = 0; i < 4; i++)
        {
            delete pyramids[i];
        }

        delete rotationArm;
    }

    void Update(float deltaTime) override
    {
        SetScale(GetUpgradeLevelScale());
        rotationArm->RotateBy(0, deltaTime * 1.5f, 0);
    }

    void Render(LineRenderer* renderer, Camera* camera) override
    {
        IBuilding::Render(renderer, camera);
        for (int i = 0; i < 4; i++)
        {
            pyramids[i]->Render(renderer, camera, baseColor);
        }
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        for (int i = 0; i < 4; i++)
        {
            pyramids[i]->SetScale(renderScale / 1.8f);
            pyramids[i]->RotateBy(0, 0, PI);
        }

        pyramids[0]->MoveToLocal(Float3(renderScale / 3.f, renderScale / 3.0f, 0));
        pyramids[1]->MoveToLocal(Float3(-renderScale / 3.f, renderScale / 3.0f, 0));
        pyramids[2]->MoveToLocal(Float3(0, renderScale / 3.0f, -renderScale / 3.f));
        pyramids[3]->MoveToLocal(Float3(0, renderScale / 3.0f, renderScale / 3.f));
    }

    std::string GetName() override
    {
        return "Burst Tower";
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

private:
    Transform* rotationArm;
    DrawablePyramid* pyramids[4];
};
