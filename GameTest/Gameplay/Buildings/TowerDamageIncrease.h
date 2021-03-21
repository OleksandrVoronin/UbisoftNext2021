#pragma once
#include "TowerAOE.h"
#include "TowerBasic.h"
#include "TowerFrost.h"
#include "../../Drawables/DrawableRhombus.h"

class TowerDamageIncrease : public IAreaTower
{
public:
    TowerDamageIncrease() : TowerDamageIncrease(Float3(1.f, 0.5f, 0.1f))
    {
    }

    TowerDamageIncrease(Float3 baseColor) : IAreaTower(baseColor)
    {
        rotationArm = new Transform(Float3(), this);
        rhombuses[0] = new DrawableRhombus(Float3(), rotationArm, 0.5f);
        rhombuses[1] = new DrawableRhombus(Float3(), rotationArm, 0.5f);
    }

    ~TowerDamageIncrease()
    {
        delete rhombuses[0];
        delete rhombuses[1];
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
        rhombuses[0]->Render(renderer, camera, baseColor);
        rhombuses[1]->Render(renderer, camera, baseColor);
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        rhombuses[0]->SetScale(renderScale / 1.8f);
        rhombuses[1]->SetScale(renderScale / 1.8f);
        rhombuses[0]->MoveToLocal(Float3(renderScale / 4.f, renderScale / 3.0f, 0));
        rhombuses[1]->MoveToLocal(Float3(-renderScale / 4.f, renderScale / 3.0f, 0));
    }

    std::string GetName() override
    {
        return "Corruption Tower";
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
    DrawableRhombus* rhombuses[2];
};
