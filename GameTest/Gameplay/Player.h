#pragma once

#include "../Camera.h"

class TowerDefenseArena;

class Player
{
public:
    Float3 gridSelection{-1, -1};
    Float3 mouseXZProjection;

    Player(TowerDefenseArena* arena) : arena(arena)
    {
    }

    void Update()
    {
        CalculateGridSelection();
    }

    void Render() const
    {
        RenderMouseXZProjection();
        RenderGridSelection();
    }

private:
    TowerDefenseArena* arena;

    void CalculateGridSelection();

    void RenderMouseXZProjection() const;

    void RenderGridSelection() const;
};
