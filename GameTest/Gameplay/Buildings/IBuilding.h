#pragma once
#include "../../Camera.h"
#include "../../LineRenderer.h"

class IBuilding : public Transform
{
public:
    IBuilding(float renderScale, Float3 baseColor) : baseColor{baseColor}, renderScale{renderScale}
    {
    }

    IBuilding() : baseColor{0, 0, 0}, renderScale{1}
    {
    }

    virtual ~IBuilding() = default;

    virtual void Render(LineRenderer* renderer, Camera* camera)
    {
        RenderBuildingBase(renderer, camera);
    }

    virtual void Update(float deltaTime) = 0;

private:
    Float3 baseColor;
    float renderScale = 1.0f;

    void RenderBuildingBase(LineRenderer* renderer, Camera* camera) const
    {
        const float baseSide = 0.5f * renderScale;
        const float upperBaseSide = 0.4f * renderScale;
        const float upperBaseElevation = 0.2f * renderScale;

        const Float3 baseA = camera->WorldToCamera(worldPosition + right * baseSide + forward * baseSide);
        const Float3 baseB = camera->WorldToCamera(worldPosition - right * baseSide + forward * baseSide);
        const Float3 baseC = camera->WorldToCamera(worldPosition - right * baseSide - forward * baseSide);
        const Float3 baseD = camera->WorldToCamera(worldPosition + right * baseSide - forward * baseSide);

        const Float3 upperBaseA = camera->
            WorldToCamera(worldPosition + right * upperBaseSide + forward * upperBaseSide + up * upperBaseElevation);
        const Float3 upperBaseB = camera->
            WorldToCamera(worldPosition - right * upperBaseSide + forward * upperBaseSide + up * upperBaseElevation);
        const Float3 upperBaseC = camera->
            WorldToCamera(worldPosition - right * upperBaseSide - forward * upperBaseSide + up * upperBaseElevation);
        const Float3 upperBaseD = camera->
            WorldToCamera(worldPosition + right * upperBaseSide - forward * upperBaseSide + up * upperBaseElevation);

        renderer->DrawLineFogApplied(&baseA, &baseB, baseColor);
        renderer->DrawLineFogApplied(&baseB, &baseC, baseColor);
        renderer->DrawLineFogApplied(&baseC, &baseD, baseColor);
        renderer->DrawLineFogApplied(&baseD, &baseA, baseColor);

        renderer->DrawLineFogApplied(&baseA, &upperBaseA, baseColor);
        renderer->DrawLineFogApplied(&baseB, &upperBaseB, baseColor);
        renderer->DrawLineFogApplied(&baseC, &upperBaseC, baseColor);
        renderer->DrawLineFogApplied(&baseD, &upperBaseD, baseColor);

        renderer->DrawLineFogApplied(&upperBaseA, &upperBaseB, baseColor);
        renderer->DrawLineFogApplied(&upperBaseB, &upperBaseC, baseColor);
        renderer->DrawLineFogApplied(&upperBaseC, &upperBaseD, baseColor);
        renderer->DrawLineFogApplied(&upperBaseD, &upperBaseA, baseColor);
    }
};
