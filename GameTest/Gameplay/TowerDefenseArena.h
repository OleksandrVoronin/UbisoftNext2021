#pragma once

#include <unordered_set>

#include "ArenaTile.h"
#include "PlayerState.h"
#include "../Utils/Camera.h"
#include "../Utils/LineRenderer.h"
#include "../Math/Float3.h"
#include "Enemies/IEnemy.h"

#include "Enemies/IWaypointMover.h"

class IBuilding;
class PlayerInput;
class HUD;
class WaveChoreographer;

class TowerDefenseArena
{
public:
    const float arenaSizeX = 20;
    const float arenaSizeZ = 20;
    const int cellCountX;
    const int cellCountZ;
    const float tileSizeX = arenaSizeX / static_cast<float>(cellCountX);
    const float tileSizeZ = arenaSizeZ / static_cast<float>(cellCountZ);

    TowerDefenseArena(int cellsX, int cellsZ, std::vector<Float3> map);

    ~TowerDefenseArena();

    void Update(float deltaTime);

    void Render() const;

    int GetEnemyCount() const
    {
        return enemies.size();
    }

    void AddEnemy(IEnemy* newEnemy)
    {
        enemies.insert(newEnemy);
    }

    void DeleteEnemy(IEnemy* enemy)
    {
        ArenaTile* currentTile = enemy->GetCurrentTile();
        if (currentTile != nullptr)
        {
            currentTile->RemoveEnemy(enemy);
        }
        enemies.erase(enemy);

        delete enemy;
    }

    void AddBuilding(int x, int y, IBuilding* building);

    void RemoveBuilding(int x, int y);

    PlayerState* GetPlayerState() const
    {
        return playerState;
    }

    PlayerInput* GetPlayerInput() const
    {
        return playerInput;
    }

    LineRenderer* GetLineRenderer() const
    {
        return lineRenderer;
    }

    Camera* GetCamera() const
    {
        return camera;
    }

    float GetTimeElapsed() const
    {
        return timeElapsed;
    }

    float GetTimeScale() const
    {
        return timeScale;
    }

    std::vector<Float3>* GetPath()
    {
        return &path;
    }

    Transform* GetWorldRoot() const
    {
        return worldRoot;
    }

    WaveChoreographer* GetWaveChoreographer() const {
        return waveChoreographer;
    }

    Float3 GetTilePosition(int x, int y)
    {
        ArenaTile* tile = GetTileByIndices(x, y);
        if (tile)
        {
            return Float3(tile->GetTileCoordinates().x * tileSizeX, 0, tile->GetTileCoordinates().z * tileSizeZ);
        }

        return Float3(-1, -1, -1);
    }

    ArenaTile* GetTileByIndices(int x, int y)
    {
        if (x < 0 || y < 0 || x >= cellCountX || y >= cellCountZ) return nullptr;

        return &tiles[x][y];
    }

    ArenaTile* GetTileByPosition(Float3 position)
    {
        // Round down to tiles.
        position = Float3(floor(position.x / tileSizeX), 0, floor(position.z / tileSizeZ));
        return GetTileByIndices(static_cast<int>(position.x), static_cast<int>(position.z));
    }

    void RenderSquareOnTile(int x, int z, float size, Float3 color, bool fogApplied = true, float yOffset = 0) const
    {
        if (x < 0 || z < 0 || x >= cellCountX || z >= cellCountZ) return;

        const float sizeX = tileSizeX * size / 2.f;
        const float sizeZ = tileSizeZ * size / 2.f;
        const Float3 midPoint = Float3(tileSizeX * (x + 0.5f), yOffset, tileSizeZ * (z + 0.5f));
        const Float3 a = camera->WorldToCamera(midPoint + Float3(-sizeX, 0, -sizeZ));
        const Float3 b = camera->WorldToCamera(midPoint + Float3(-sizeX, 0, sizeZ));
        const Float3 c = camera->WorldToCamera(midPoint + Float3(sizeX, 0, sizeZ));
        const Float3 d = camera->WorldToCamera(midPoint + Float3(sizeX, 0, -sizeZ));

        if (fogApplied)
        {
            lineRenderer->DrawLineFogApplied(&a, &b, color);
            lineRenderer->DrawLineFogApplied(&b, &c, color);
            lineRenderer->DrawLineFogApplied(&c, &d, color);
            lineRenderer->DrawLineFogApplied(&d, &a, color);
        }
        else
        {
            lineRenderer->DrawLine(&a, &b, color);
            lineRenderer->DrawLine(&b, &c, color);
            lineRenderer->DrawLine(&c, &d, color);
            lineRenderer->DrawLine(&d, &a, color);
        }
    }

private:
    const Float3 gridColor{0.36f, 0.0f, 0.0f};

    Transform* worldRoot;
    Camera* camera;
    LineRenderer* lineRenderer;
    PlayerInput* playerInput;
    HUD* hud;
    PlayerState* playerState;
    WaveChoreographer* waveChoreographer;

    std::vector<Float3> path;
    std::vector<std::vector<ArenaTile>> tiles;
    std::unordered_set<IBuilding*> buildings;
    std::unordered_set<IEnemy*> enemies;

    float timeScale = 1.f;
    float timeElapsed = 0.f;

    void ParseMap(std::vector<Float3> map);

    void VisualizePath() const
    {
        //const Float3 visualizationOffset = Float3(tileSizeX / 2, 0.1f, tileSizeZ / 2);

        for (int i = 0; i < path.size() - 1; i++)
        {
            Float3 a = camera->WorldToCamera(path[i]);
            Float3 b = camera->WorldToCamera(path[i + 1]);

            lineRenderer->DrawLineFogApplied(&a, &b, Float3(1.0f, 0, 0));
        }
    }

    // Renders grid lines with continuous lines only breaking them where necessary (rather than rendering squares outside of path, causing tons of overdraw)
    void RenderArena() const
    {
        // Render horizontal grid lines
        for (int z = 0; z <= cellCountZ; z++)
        {
            int lineStart = 0;
            for (int x = 0; x < cellCountX; x++)
            {
                boolean needsLine = false;

                if (z - 1 < 0)
                {
                    if (!tiles[x][z].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }
                else if (z >= cellCountZ)
                {
                    if (!tiles[x][z - 1].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }
                else
                {
                    if ((tiles[x][z].GetIsPartOfThePath() == false &&
                            tiles[x][z - 1].GetIsPartOfThePath() == false) ||
                        tiles[x][z].GetIsPartOfThePath() != tiles[x][z - 1].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }

                if (!needsLine || x == cellCountX - 1)
                {
                    // If this is the last horizontal cell - push the row to the end.
                    if (x == cellCountX - 1)
                    {
                        x++;
                    }

                    if (x != lineStart)
                    {
                        const Float3 a = camera->WorldToCamera(Float3(tileSizeX * (x), 0, tileSizeZ * (z)));
                        const Float3 b = camera->WorldToCamera(Float3(tileSizeX * (lineStart), 0, tileSizeZ * (z)));
                        lineRenderer->DrawLineFogApplied(&a, &b, gridColor);
                    }
                    lineStart = x + 1;
                }
            }
        }

        // Render vertical grid lines
        for (int x = 0; x <= cellCountX; x++)
        {
            int lineStart = 0;
            for (int z = 0; z < cellCountZ; z++)
            {
                boolean needsLine = false;

                if (x - 1 < 0)
                {
                    if (!tiles[x][z].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }
                else if (x >= cellCountX)
                {
                    if (!tiles[x - 1][z].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }
                else
                {
                    if ((tiles[x][z].GetIsPartOfThePath() == false &&
                            tiles[x - 1][z].GetIsPartOfThePath() == false) ||
                        tiles[x][z].GetIsPartOfThePath() != tiles[x - 1][z].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }

                if (!needsLine || z == cellCountZ - 1)
                {
                    // If this is the last horizontal cell - push the row to the end.
                    if (z == cellCountZ - 1)
                    {
                        z++;
                    }

                    if (z != lineStart)
                    {
                        const Float3 a = camera->WorldToCamera(Float3(tileSizeX * (x), 0, tileSizeZ * (z)));
                        const Float3 b = camera->WorldToCamera(Float3(tileSizeX * (x), 0, tileSizeZ * (lineStart)));
                        lineRenderer->DrawLineFogApplied(&a, &b, gridColor);
                    }
                    lineStart = z + 1;
                }
            }
        }

        //Render vertical downwards lines
        for (int z = 0; z < cellCountZ; z++)
        {
            for (int x = 0; x < cellCountX; x++)
            {
                boolean needsLine = false;

                if (z - 1 < 0)
                {
                    if (!tiles[x][z].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }
                else
                {
                    if (!tiles[x][z].GetIsPartOfThePath() &&
                        tiles[x][z - 1].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }

                if (needsLine)
                {
                    if (x == 0 || z == 0 || tiles[x - 1][z - 1].GetIsPartOfThePath())
                    {
                        const Float3 a = camera->WorldToCamera(Float3(tileSizeX * (x), 0, tileSizeZ * (z)));
                        const Float3 b = camera->WorldToCamera(
                            Float3(tileSizeX * (x), -tileSizeZ * 1.1f, tileSizeZ * (z)));
                        lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, lineRenderer->fogColor, 4.f);
                    }

                    if (x == cellCountX - 1 || z == 0 || tiles[x + 1][z - 1].GetIsPartOfThePath())
                    {
                        const Float3 c = camera->WorldToCamera(Float3(tileSizeX * (x + 1), 0, tileSizeZ * (z)));
                        const Float3 d = camera->WorldToCamera(
                            Float3(tileSizeX * (x + 1), -tileSizeZ * 1.1f, tileSizeZ * (z)));
                        lineRenderer->DrawGradientLineFogApplied(&c, &d, gridColor, lineRenderer->fogColor, 4.f);
                    }
                }
            }
        }

        //Render vertical side lines
        for (int x = 0; x < cellCountX; x++)
        {
            for (int z = 0; z < cellCountZ; z++)
            {
                boolean needsLeftLine = false;
                boolean needsRightLine = false;

                if (x < cellCountX / 2 - 1 &&
                    (!tiles[x][z].GetIsPartOfThePath() &&
                        tiles[x + 1][z].GetIsPartOfThePath()))
                {
                    needsLeftLine = true;
                }

                if (x >= cellCountX / 2 && x + 1 < cellCountX &&
                    (tiles[x][z].GetIsPartOfThePath() &&
                        !tiles[x + 1][z].GetIsPartOfThePath()))
                {
                    needsRightLine = true;
                }

                if (needsLeftLine)
                {
                    const Float3 a = camera->WorldToCamera(Float3(tileSizeX * (x + 1), 0, tileSizeZ * (z + 1)));
                    const Float3 b = camera->WorldToCamera(
                        Float3(tileSizeX * (x + 1), -tileSizeZ * 1.1f, tileSizeZ * (z + 1)));
                    lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, lineRenderer->fogColor, 4.f);
                }

                if (needsRightLine)
                {
                    const Float3 a = camera->WorldToCamera(Float3(tileSizeX * (x + 1), 0, tileSizeZ * (z + 1)));
                    const Float3 b = camera->WorldToCamera(
                        Float3(tileSizeX * (x + 1), -tileSizeZ * 1.1f, tileSizeZ * (z + 1)));
                    lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, lineRenderer->fogColor, 4.f);
                }
            }
        }
    }
};
