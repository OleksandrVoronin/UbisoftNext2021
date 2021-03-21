#pragma once

#include <unordered_set>

#include "ArenaTile.h"
#include "PlayerState.h"
#include "../Utils/Camera.h"
#include "../Utils/LineRenderer.h"
#include "../Math/Float3.h"
#include "Buildings/CoreBuilding.h"

class PlayerInput;
class HUD;

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

    void Update(float deltaTime) const;

    void Render() const;

    void AddBuilding(int x, int y, IBuilding* building)
    {
        GetTile(x, y)->AddBuilding(building);
        building->SetRenderScale(min(tileSizeX, tileSizeZ));
        building->SetParent(worldRoot);
        building->MoveToLocal(GetTilePosition(x, y) + Float3(tileSizeX / 2.0f, 0, tileSizeZ / 2.0f));
        buildings.insert(building);
    }

    void RemoveBuilding(int x, int y)
    {
        ArenaTile* tile = GetTile(x, y);
        if (tile)
        {
            IBuilding* building = tile->GetBuilding();
            buildings.erase(building);
            tile->RemoveBuilding();
            delete building;
        }
    }

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

    Float3 GetTilePosition(int x, int y)
    {
        ArenaTile* tile = GetTile(x, y);
        if (tile)
        {
            return Float3(tile->GetTileCoordinates().x * tileSizeX, 0, tile->GetTileCoordinates().z * tileSizeZ);
        }

        return Float3(-1, -1, -1);
    }

    ArenaTile* GetTile(int x, int y)
    {
        if (x < 0 || y < 0 || x >= cellCountX || y >= cellCountZ) return nullptr;

        return &tiles[x][y];
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

    std::vector<Float3> path;
    std::vector<std::vector<ArenaTile>> tiles;
    std::unordered_set<IBuilding*> buildings;

    void ParseMap(std::vector<Float3> map)
    {
        // Convert given path into a list of coordinates.
        for (int i = 0; i < map.size(); i++)
        {
            path.push_back(Float3(map[i].x * tileSizeX, 0, map[i].y * tileSizeZ));
        }

        // Build a matrix and figure out every tile of the path connecting path points.
        std::vector<std::vector<boolean>> pathMap(cellCountX, std::vector<boolean>(cellCountZ, false));
        Float3 currentPosition = Float3(map[0].x, 0, map[0].y);
        int currentIndex = 0;

        while (currentIndex != path.size() - 1)
        {
            pathMap[static_cast<int>(round(currentPosition.x))][static_cast<int>(round(currentPosition.z))] = true;

            Float3 target = Float3(map[currentIndex + 1].x, 0, map[currentIndex + 1].y);
            if (currentPosition.x == target.x && currentPosition.z == target.z)
            {
                currentIndex++;
                continue;
            }

            const Float3 direction = (target - currentPosition).Normalized();
            currentPosition = Float3(currentPosition.x + direction.x,
                                     currentPosition.y + direction.y,
                                     currentPosition.z + direction.z);
        }

        // Use matrix above to generate actual tile objects.
        tiles.resize(cellCountX);
        for (int x = 0; x < cellCountX; x++)
        {
            for (int z = 0; z < cellCountZ; z++)
            {
                tiles[x].push_back(ArenaTile{Float3(static_cast<float>(x), 0, static_cast<float>(z)), pathMap[x][z]});
            }
        }

        AddBuilding(static_cast<int>(path[path.size() - 1].x), static_cast<int>(path[path.size() - 1].z),
                    new CoreBuilding());

        path.insert(path.begin(), Float3(static_cast<int>(map[0].x) * tileSizeX, 0, (map[0].y + 2) * tileSizeZ));
    }

    void VisualizePath() const
    {
        const Float3 visualizationOffset = Float3(tileSizeX / 2, 0.1f, tileSizeZ / 2);

        for (int i = 0; i < path.size() - 1; i++)
        {
            Float3 a = camera->WorldToCamera(path[i] + visualizationOffset);
            Float3 b = camera->WorldToCamera(path[i + 1] + visualizationOffset);

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
                        const Float3 b = camera->WorldToCamera(Float3(tileSizeX * (x), -tileSizeZ * 1.1f, tileSizeZ * (z)));
                        lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, lineRenderer->fogColor, 4.f);
                    }

                    if (x == cellCountX - 1 || z == 0 || tiles[x + 1][z - 1].GetIsPartOfThePath())
                    {
                        const Float3 c = camera->WorldToCamera(Float3(tileSizeX * (x + 1), 0, tileSizeZ * (z)));
                        const Float3 d = camera->WorldToCamera(Float3(tileSizeX * (x + 1), -tileSizeZ * 1.1f, tileSizeZ * (z)));
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
                    const Float3 b = camera->WorldToCamera(Float3(tileSizeX * (x + 1), -tileSizeZ * 1.1f, tileSizeZ * (z + 1)));
                    lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, lineRenderer->fogColor, 4.f);
                }

                if (needsRightLine)
                {
                    const Float3 a = camera->WorldToCamera(Float3(tileSizeX * (x + 1), 0, tileSizeZ * (z + 1)));
                    const Float3 b = camera->WorldToCamera(Float3(tileSizeX * (x + 1), -tileSizeZ * 1.1f, tileSizeZ * (z + 1)));
                    lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, lineRenderer->fogColor, 4.f);
                }
            }
        }
    }
};
