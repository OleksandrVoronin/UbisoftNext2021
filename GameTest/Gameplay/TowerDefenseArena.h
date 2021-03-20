#pragma once

#include "ArenaTile.h"
#include "../Camera.h"
#include "../LineRenderer.h"
#include "../Math/Float3.h"

class Player;

class TowerDefenseArena
{
public:
    const float worldSizeX = 20;
    const float worldSizeZ = 20;
    const int cellsX;
    const int cellsZ;
    const float stepX = worldSizeX / static_cast<float>(cellsX);
    const float stepZ = worldSizeZ / static_cast<float>(cellsZ);

    TowerDefenseArena(int cellsX, int cellsZ, Float3* map);

    ~TowerDefenseArena();

    void Update(float deltaTime) const;

    void Render() const;

    LineRenderer* GetLineRenderer()
    {
        return lineRenderer;
    }

    Camera* GetCamera()
    {
        return camera;
    }

    void RenderSquare(int x, int z, float size, Float3 color, bool fogApplied = true, float yOffset = 0) const
    {
        if (x < 0 || z < 0 || x >= cellsX || z >= cellsZ) return;

        const float sizeX = stepX * size;
        const float sizeZ = stepZ * size;
        const Float3 midPoint = Float3(stepX * (x + 0.5f), yOffset, stepZ * (z + 0.5f));
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
            lineRenderer->DrawLineSingleColor(a, b, color);
            lineRenderer->DrawLineSingleColor(b, c, color);
            lineRenderer->DrawLineSingleColor(c, d, color);
            lineRenderer->DrawLineSingleColor(d, a, color);
        }
    }

private:
    Transform* root;
    Camera* camera;
    LineRenderer* lineRenderer;
    Player* player;

    const Float3 gridColor{0, 0.62745, 0.63922};

    std::vector<Float3> path;
    std::vector<std::vector<ArenaTile>> tiles;

    // Renders grid lines with continuous lines only breaking them where necessary (rather than rendering squares outside of path, causing tons of overdraw)
    void RenderArena() const
    {
        // Render horizontal grid lines
        for (int z = 0; z <= cellsZ; z++)
        {
            int lineStart = 0;
            for (int x = 0; x < cellsX; x++)
            {
                boolean needsLine = false;

                if (z - 1 < 0)
                {
                    if (!tiles[x][z].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }
                else if (z >= cellsZ)
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

                if (!needsLine || x == cellsX - 1)
                {
                    // If this is the last horizontal cell - push the row to the end.
                    if (x == cellsX - 1)
                    {
                        x++;
                    }

                    if (x != lineStart)
                    {
                        const Float3 a = camera->WorldToCamera(Float3(stepX * (x), 0, stepZ * (z)));
                        const Float3 b = camera->WorldToCamera(Float3(stepX * (lineStart), 0, stepZ * (z)));
                        lineRenderer->DrawLineFogApplied(&a, &b, gridColor);
                    }
                    lineStart = x + 1;
                }
            }
        }

        // Render vertical grid lines
        for (int x = 0; x <= cellsX; x++)
        {
            int lineStart = 0;
            for (int z = 0; z < cellsZ; z++)
            {
                boolean needsLine = false;

                if (x - 1 < 0)
                {
                    if (!tiles[x][z].GetIsPartOfThePath())
                    {
                        needsLine = true;
                    }
                }
                else if (x >= cellsX)
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

                if (!needsLine || z == cellsZ - 1)
                {
                    // If this is the last horizontal cell - push the row to the end.
                    if (z == cellsZ - 1)
                    {
                        z++;
                    }

                    if (z != lineStart)
                    {
                        const Float3 a = camera->WorldToCamera(Float3(stepX * (x), 0, stepZ * (z)));
                        const Float3 b = camera->WorldToCamera(Float3(stepX * (x), 0, stepZ * (lineStart)));
                        lineRenderer->DrawLineFogApplied(&a, &b, gridColor);
                    }
                    lineStart = z + 1;
                }
            }
        }

        //Render vertical downwards lines
        for (int z = 0; z < cellsZ; z++)
        {
            for (int x = 0; x < cellsX; x++)
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
                        const Float3 a = camera->WorldToCamera(Float3(stepX * (x), 0, stepZ * (z)));
                        const Float3 b = camera->WorldToCamera(Float3(stepX * (x), -stepZ * 1.2f, stepZ * (z)));
                        lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, Float3(), 4.f);
                    }

                    if (x == cellsX - 1 || z == 0 || tiles[x + 1][z - 1].GetIsPartOfThePath())
                    {
                        const Float3 c = camera->WorldToCamera(Float3(stepX * (x + 1), 0, stepZ * (z)));
                        const Float3 d = camera->WorldToCamera(Float3(stepX * (x + 1), -stepZ * 1.2f, stepZ * (z)));
                        lineRenderer->DrawGradientLineFogApplied(&c, &d, gridColor, Float3(), 4.f);
                    }
                }
            }
        }

        //Render vertical side lines
        for (int x = 0; x < cellsX; x++)
        {
            for (int z = 0; z < cellsZ; z++)
            {
                boolean needsLeftLine = false;
                boolean needsRightLine = false;

                if (x < cellsX / 2 - 1 &&
                    (!tiles[x][z].GetIsPartOfThePath() &&
                        tiles[x + 1][z].GetIsPartOfThePath()))
                {
                    needsLeftLine = true;
                }

                if (x >= cellsX / 2 && x + 1 < cellsX &&
                    (tiles[x][z].GetIsPartOfThePath() &&
                        !tiles[x + 1][z].GetIsPartOfThePath()))
                {
                    needsRightLine = true;
                }

                if (needsLeftLine)
                {
                    const Float3 a = camera->WorldToCamera(Float3(stepX * (x + 1), 0, stepZ * (z + 1)));
                    const Float3 b = camera->WorldToCamera(Float3(stepX * (x + 1), -stepZ * 1.2f, stepZ * (z + 1)));
                    lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, Float3(), 4.f);
                }

                if (needsRightLine)
                {
                    const Float3 a = camera->WorldToCamera(Float3(stepX * (x + 1), 0, stepZ * (z + 1)));
                    const Float3 b = camera->WorldToCamera(Float3(stepX * (x + 1), -stepZ * 1.2f, stepZ * (z + 1)));
                    lineRenderer->DrawGradientLineFogApplied(&a, &b, gridColor, Float3(), 4.f);
                }
            }
        }
    }

    void VisualizePath() const
    {
        const Float3 visualizationOffset = Float3(stepX / 2, 0.1f, stepZ / 2);

        for (int i = 0; i < path.size() - 1; i++)
        {
            Float3 a = camera->WorldToCamera(path[i] + visualizationOffset);
            Float3 b = camera->WorldToCamera(path[i + 1] + visualizationOffset);

            lineRenderer->DrawLineFogApplied(&a, &b, Float3(1.0f, 1.0f, 0));
        }
    }

    void ParseMap(Float3* map)
    {
        std::vector<std::vector<boolean>> pathMap(cellsX, std::vector<boolean>(cellsZ, false));

        // TODO: this is a big weird fix it, maps should be passed in vectors or sometihng
        for (int i = 0; i < 11; i++)
        {
            path.push_back(Float3(map[i].x * stepX, 0, map[i].y * stepZ));
        }

        Float3 currentPosition = Float3(map[0].x, 0, map[0].y);
        int currentIndex = 0;

        while (currentIndex != path.size() - 1)
        {
            pathMap[(int)round(currentPosition.x)][(int)round(currentPosition.z)] = true;

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

        tiles.resize(cellsX);
        for (int x = 0; x < cellsX; x++)
        {
            for (int z = 0; z < cellsZ; z++)
            {
                tiles[x].push_back(ArenaTile{Float3(x, 0, z), pathMap[x][z]});
            }
        }

        path.insert(path.begin(), Float3(map[0].x * stepX, 0, (map[0].y + 2) * stepZ));
    }
};
