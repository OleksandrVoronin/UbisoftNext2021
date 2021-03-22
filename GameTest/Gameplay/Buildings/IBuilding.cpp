#include "stdafx.h"
#include "IBuilding.h"
#include "../TowerDefenseArena.h"

void IBuilding::Render(LineRenderer* renderer, Camera* camera)
{
    RenderBuildingBase(renderer, camera);
}

void IBuilding::RenderRange(LineRenderer* renderer, Camera* camera)
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

void IBuilding::SetRenderScale(float renderScale)
{
    this->renderScale = renderScale;
}

void IBuilding::SetTile(ArenaTile* tile)
{
    this->tile = tile;
}

void IBuilding::SetArena(TowerDefenseArena* arena)
{
    this->arena = arena;
}

boolean IBuilding::IsUpgradeable() const
{
    return GetLevel() < GetMaxLevel();
}

int IBuilding::GetLevel() const
{
    return level;
}

float IBuilding::GetUpgradeLevelScale() const
{
    return 1.0f;
}

void IBuilding::UpdateTilesInRange()
{
    tilesInRange.clear();

    for (int x = -GetRange(); x <= GetRange(); x++)
    {
        for (int y = -GetRange(); y <= GetRange(); y++)
        {
            tilesInRange.insert(arena->GetTileByIndices(static_cast<int>(this->tile->GetTileCoordinates().x) + x,
                                                        static_cast<int>(this->tile->GetTileCoordinates().z) + y));
        }
    }
}

void IBuilding::RenderBuildingBase(LineRenderer* renderer, Camera* camera) const
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
