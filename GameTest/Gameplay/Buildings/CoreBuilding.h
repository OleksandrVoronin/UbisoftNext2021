#pragma once
#include "IBuilding.h"
#include "../../Utils/Camera.h"
#include "../../Utils/LineRenderer.h"
#include "../../Drawables/DrawablePyramid.h"

class CoreBuilding : public IBuilding
{
public:
    CoreBuilding() : CoreBuilding(Float3(0, 0.7f, 0))
    {
    }

    CoreBuilding(Float3 baseColor)
        : IBuilding(baseColor)
    {
        pyramid = new DrawablePyramid(Float3(), this, 1);
    }

    ~CoreBuilding()
    {
        delete pyramid;
    }

    void Render(LineRenderer* renderer, Camera* camera) override
    {
        IBuilding::Render(renderer, camera);
        pyramid->Render(renderer, camera, baseColor);
    }

    void Update(float deltaTime) override
    {
        pyramid->MoveByLocal(Float3(0, cos((float)GetCounter() / 2000.f) * renderScale * 0.001f, 0));
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        pyramid->SetScale(renderScale / 2.0f);
        pyramid->MoveToLocal(Float3(0, renderScale / 3.0f, 0));
    }

    std::string GetName() override
    {
        return "Core building";
    }

    std::vector<std::string> GetDescription() override
    {
        return {
            "This is the core building, not playable",
            "Doesn't need a description."
        };
    }

    int GetCost() override
    {
        return 0;
    }

    int GetRange() override
    {
        return 0;
    }

    float GetDamage() override
    {
        return 0;
    }

    float GetUpgradeLevelScale() const override
    {
        return 1.0f;
    }

    int GetMaxLevel() const override
    {
        return 0;
    }

private:
    DrawablePyramid* pyramid;
};
