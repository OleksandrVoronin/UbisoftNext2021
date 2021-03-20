#pragma once
#include "IBuilding.h"
#include "../../Camera.h"
#include "../../LineRenderer.h"

class CoreBuilding : public IBuilding
{
public:
    CoreBuilding(float renderScale, Float3 baseColor)
        : IBuilding(renderScale, baseColor)
    {
    }

    void Render(LineRenderer* renderer, Camera* camera) override
    {
        IBuilding::Render(renderer, camera);
    }

    void Update(float deltaTime) override
    {
    }
};
