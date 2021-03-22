#include "stdafx.h"
#include "TowerDefenseArena.h"
#include "PlayerInput.h"
#include "WaveChoreographer.h"
#include "../UI/HUD.h"
#include "Buildings/CoreBuilding.h"
#include "Enemies/EnemySoldier.h"

TowerDefenseArena::TowerDefenseArena()
{
    playerState = new PlayerState();

    worldRoot = new Transform(Float3(0, 0, 0));

    waveChoreographer = new WaveChoreographer(this);

    camera = new Camera(worldRoot, Float3{arenaSizeX / 2, arenaSizeZ * 1.5f, -arenaSizeZ}, 1.4f);
    camera->RotateAbsolute(QUARTER_PI, 0, 0);
    camera->UpdateTransform();

    lineRenderer = new LineRenderer(Float3(1, 1, 1));

    playerInput = new PlayerInput(this);

    hud = new HUD(this);

    GenerateMap();
}

TowerDefenseArena::~TowerDefenseArena()
{
    delete camera;
    delete lineRenderer;
    delete playerInput;
    delete playerState;
    delete worldRoot;
}

void TowerDefenseArena::Update(float deltaTime)
{
    deltaTime *= timeScale;
    timeElapsed += deltaTime;

    playerInput->Update();
    hud->Update();

    if (playerInput->GetKeyPressedThisFrame('R'))
    {
        FullReset();
        return;
    }
    
    if (!playerState->IsAlive())
    {
       
        return;
    }

    waveChoreographer->Update();

    if (playerInput->GetKeyPressedThisFrame('A'))
    {
        timeScale = max(1, timeScale / 2);
    }
    else if (playerInput->GetKeyPressedThisFrame('D'))
    {
        timeScale = min(8, timeScale * 2);
    }

    for (std::unordered_set<IBuilding*>::iterator itr = buildings.begin(); itr != buildings.end(); ++itr)
    {
        (*itr)->Update(deltaTime);
    }

    std::unordered_set<IEnemy*> deadEnemies;
    for (std::unordered_set<IEnemy*>::iterator itr = enemies.begin(); itr != enemies.end(); ++itr)
    {
        if (!(*itr)->IsAlive())
        {
            deadEnemies.insert(*itr);
            continue;
        }

        (*itr)->Update(deltaTime);

        // Update tile records for faster lookups (to identify which tiles have enemies etc).
        ArenaTile* recordedCurrentTile = (*itr)->GetCurrentTile();
        ArenaTile* currentTile = GetTileByPosition((*itr)->GetWorldPosition());
        if (recordedCurrentTile != currentTile)
        {
            if (recordedCurrentTile)
            {
                recordedCurrentTile->RemoveEnemy((*itr));
            }
            if (currentTile)
            {
                currentTile->AddEnemy((*itr));
            }
            (*itr)->SetCurrentTile(currentTile);
        }
    }

    // Remove dead enemies.
    while (deadEnemies.size() > 0)
    {
        IEnemy* enemy = *(deadEnemies.begin());
        deadEnemies.erase(enemy);
        DeleteEnemy(enemy);
        playerState->DeltaCredits(1);
    }

    worldRoot->UpdateTransform();
    playerInput->LateUpdate();
}

void TowerDefenseArena::Render() const
{
    RenderArena();

    for (std::unordered_set<IBuilding*>::iterator itr = buildings.begin(); itr != buildings.end(); ++itr)
    {
        (*itr)->Render(lineRenderer, camera);
    }

    for (std::unordered_set<IEnemy*>::iterator itr = enemies.begin(); itr != enemies.end(); ++itr)
    {
        (*itr)->Render(lineRenderer, camera);
    }

    lineRenderer->RenderFrame();

    // HUD needs to render as a second pass, on top of the game
    hud->Render();
}

void TowerDefenseArena::AddBuilding(int x, int y, IBuilding* building)
{
    GetTileByIndices(x, y)->RegisterBuilding(building);
    building->SetArena(this);
    building->SetRenderScale(min(tileSizeX, tileSizeZ));
    building->SetParent(worldRoot);
    building->MoveToLocal(GetTilePosition(x, y) + Float3(tileSizeX / 2.0f, 0, tileSizeZ / 2.0f));
    buildings.insert(building);

    building->OnPlaced();
}

void TowerDefenseArena::RemoveBuilding(int x, int y)
{
    ArenaTile* tile = GetTileByIndices(x, y);
    if (tile)
    {
        IBuilding* building = tile->GetBuilding();
        buildings.erase(building);
        tile->RemoveBuilding();
        delete building;
    }
}

void TowerDefenseArena::FullReset()
{
    while (enemies.size() > 0)
    {
        IEnemy* toDelete = *(enemies.begin());
        enemies.erase(toDelete);
        delete toDelete;
    }

    while (buildings.size() > 0)
    {
        IBuilding* toDelete = *(buildings.begin());
        buildings.erase(toDelete);
        delete toDelete;
    }

    delete waveChoreographer;
    waveChoreographer = new WaveChoreographer(this);

    delete playerState;
    playerState = new PlayerState();

    GenerateMap();
}

void TowerDefenseArena::ParseMap(std::vector<Float3> map)
{
    path.clear();
    // Convert given path into a list of coordinates.
    for (int i = 0; i < map.size(); i++)
    {
        path.push_back(Float3((map[i].x + 0.5f) * tileSizeX, 0, (map[i].y + 0.5f) * tileSizeZ));
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

    for (int i = 0; i < tiles.size(); i++)
    {
        tiles[i].clear();
    }
    tiles.clear();

    // Use matrix above to generate actual tile objects.
    tiles.resize(cellCountX);
    for (int x = 0; x < cellCountX; x++)
    {
        for (int z = 0; z < cellCountZ; z++)
        {
            tiles[x].push_back(ArenaTile{Float3(static_cast<float>(x), 0, static_cast<float>(z)), pathMap[x][z]});
        }
    }

    AddBuilding(static_cast<int>(map[map.size() - 1].x), static_cast<int>(map[map.size() - 1].y),
                new CoreBuilding());

    path.insert(path.begin(), Float3(path[0].x, 0, path[0].z + 2 * tileSizeZ));
}

void TowerDefenseArena::VisualizePath() const
{
    //const Float3 visualizationOffset = Float3(tileSizeX / 2, 0.1f, tileSizeZ / 2);

    for (int i = 0; i < path.size() - 1; i++)
    {
        Float3 a = camera->WorldToCamera(path[i]);
        Float3 b = camera->WorldToCamera(path[i + 1]);

        lineRenderer->DrawLineFogApplied(&a, &b, Float3(1.0f, 0, 0));
    }
}

void TowerDefenseArena::RenderArena() const
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
