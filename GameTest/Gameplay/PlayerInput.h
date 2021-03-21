#pragma once

#include "../Utils/Camera.h"


class TowerDefenseArena;

class PlayerInput
{
public:
    Float3 GetMouseScreenPosition() const
    {
        return mouseScreenPosition;
    }

    Float3 GetGridSelection() const
    {
        return gridSelection;
    }

    Float3 GetXZProjection() const
    {
        return mouseXZProjection;
    }

    boolean GetLMousePressedThisFrame() const;
    boolean GetLMouseReleasedThisFrame() const;

    boolean GetRMousePressedThisFrame() const;
    boolean GetRMouseReleasedThisFrame() const;

    PlayerInput(TowerDefenseArena* arena);

    void Update();

    void LateUpdate();

private:
    TowerDefenseArena* arena;

    Float3 gridSelection{-1, -1};
    Float3 mouseXZProjection;
    Float3 mouseScreenPosition;

    boolean lastLMousePressedState = false;
    boolean lastRMousePressedState = false;
    boolean currentLMousePressedState = false;
    boolean currentRMousePressedState = false;

    void CalculateGridSelection();
};
