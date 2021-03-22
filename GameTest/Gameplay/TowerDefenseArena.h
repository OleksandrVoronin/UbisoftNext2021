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
    const int cellCountX = 12;
    const int cellCountZ = 12;
    const float tileSizeX = arenaSizeX / static_cast<float>(cellCountX);
    const float tileSizeZ = arenaSizeZ / static_cast<float>(cellCountZ);

    TowerDefenseArena();

    ~TowerDefenseArena();

    void Update(float deltaTime);

    void Render() const;

    size_t GetEnemyCount() const
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

    WaveChoreographer* GetWaveChoreographer() const
    {
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

    void FullReset();

    void ParseMap(std::vector<Float3> map);

    void VisualizePath() const;

    // Renders grid lines with continuous lines only breaking them where necessary (rather than rendering squares outside of path, causing tons of overdraw)
    void RenderArena() const;

    void GenerateMap()
    {
        // This was supposed to generate a map but I'm out of time.
        const int randomMap = rand() % mapVariants.size();

        ParseMap(mapVariants[randomMap]);
    }

    const std::vector<std::vector<Float3>> mapVariants{
        {
            Float3(10, 11),
            Float3(10, 8),
            Float3(6, 8),
            Float3(6, 5),
            Float3(4, 5),
            Float3(4, 10),
            Float3(1, 10),
            Float3(1, 3),
            Float3(10, 3),
            Float3(10, 1),
            Float3(1, 1)
        },
        {
            Float3(1, 11),
            Float3(1, 10),
            Float3(4, 10),
            Float3(4, 8),
            Float3(7, 8),
            Float3(7, 10),
            Float3(10, 10),
            Float3(10, 6),
            Float3(1, 6),
            Float3(1, 1),
            Float3(4, 1),
            Float3(4, 4),
            Float3(9, 4),
            Float3(9, 1),
            Float3(10, 1)
        },
        {
            Float3(5, 11),
            Float3(5, 10),
            Float3(1, 10),
            Float3(1, 7),
            Float3(3, 7),
            Float3(3, 8),
            Float3(7, 8),
            Float3(7, 10),
            Float3(10, 10),
            Float3(10, 1),
            Float3(3, 1),
            Float3(3, 5),
            Float3(7, 5),
            Float3(7, 3),
            Float3(1, 3)
        },
        {
            Float3(8, 11),
            Float3(8, 7),
            Float3(2, 7),
            Float3(2, 10),
            Float3(6, 10),
            Float3(6, 1),
            Float3(2, 1),
            Float3(2, 4),
            Float3(8, 4),
            Float3(8, 1),
            Float3(10, 1),
        },
        {
            Float3(1, 11),
            Float3(1, 8),
            Float3(10, 8),
            Float3(10, 5),
            Float3(1, 5),
            Float3(1, 1),
            Float3(9, 1),
            Float3(9, 3),
            Float3(4, 3)
        }

    };
};
