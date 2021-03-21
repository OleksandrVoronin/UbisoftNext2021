#pragma once
#include "../../Camera.h"
#include "../../LineRenderer.h"

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

    virtual void SetRenderScale(float renderScale)
    {
        this->renderScale = renderScale;
    }

    virtual void Render(LineRenderer* renderer, Camera* camera)
    {
        RenderBuildingBase(renderer, camera);
    }

    virtual void RenderRange(LineRenderer* renderer, Camera* camera)
    {
        if (GetRange() <= 0) return;

        const float rangeProportionalOffset = renderScale / 200.f / GetRange();

        // Render a few times with offsets to mimic thicker lines
        for (int i = 0; i < 3; i++)
        {
            const float halfOfSide = ((GetRange() * 2 + 1) * renderScale) / 2.f * (1 - rangeProportionalOffset * i);

            const Float3 baseA = camera->WorldToCamera(
                worldPosition + right * halfOfSide + forward * halfOfSide + up * rangeProportionalOffset);
            const Float3 baseB = camera->WorldToCamera(
                worldPosition - right * halfOfSide + forward * halfOfSide + up * rangeProportionalOffset);
            const Float3 baseC = camera->WorldToCamera(
                worldPosition - right * halfOfSide - forward * halfOfSide + up * rangeProportionalOffset);
            const Float3 baseD = camera->WorldToCamera(
                worldPosition + right * halfOfSide - forward * halfOfSide + up * rangeProportionalOffset);

            renderer->DrawLine(&baseA, &baseB, baseColor);
            renderer->DrawLine(&baseB, &baseC, baseColor);
            renderer->DrawLine(&baseC, &baseD, baseColor);
            renderer->DrawLine(&baseD, &baseA, baseColor);
        }
    }

    boolean IsUpgradeable() const
    {
        return GetLevel() < GetMaxLevel();
    }

    virtual int GetMaxLevel() const
    {
        return 0;
    }

    int GetLevel() const
    {
        return level;
    }

    virtual int GetSellPrice()
    {
        return GetCost() / 2;
    }

    virtual void Upgrade()
    {
        if (level < GetMaxLevel())
        {
            level++;
        }
    }

    virtual void Update(float deltaTime) = 0;

    virtual int GetCost() = 0;

    virtual std::string GetName() = 0;

    virtual std::vector<std::string> GetDescription() = 0;

    virtual int GetRange() = 0;

    virtual float GetDamage() = 0;

    virtual float GetUpgradeLevelScale() const
    {
        return 1.0f;
    }

protected:
    Float3 baseColor;
    float renderScale = 1.0f;
    int level = 0;

    void RenderBuildingBase(LineRenderer* renderer, Camera* camera) const
    {
        const float baseSide = 0.45f * renderScale * GetUpgradeLevelScale();
        const float upElevation = 0.015f * renderScale * GetUpgradeLevelScale();
        const float upperBaseSide = 0.38f * renderScale * GetUpgradeLevelScale();
        const float upperBaseElevation = 0.15f * renderScale * GetUpgradeLevelScale();

        const Float3 baseA = camera->WorldToCamera(
            worldPosition + right * baseSide + forward * baseSide + up * upElevation);
        const Float3 baseB = camera->WorldToCamera(
            worldPosition - right * baseSide + forward * baseSide + up * upElevation);
        const Float3 baseC = camera->WorldToCamera(
            worldPosition - right * baseSide - forward * baseSide + up * upElevation);
        const Float3 baseD = camera->WorldToCamera(
            worldPosition + right * baseSide - forward * baseSide + up * upElevation);

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
