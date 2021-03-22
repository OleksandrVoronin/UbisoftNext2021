#pragma once

#include <set>
#include <unordered_map>
#include <unordered_set>


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

    PlayerInput(TowerDefenseArena* arena);

    boolean GetKeyPressedThisFrame(int keycode);
    boolean GetKeyReleasedThisFrame(int keycode);

    void Update();

    void LateUpdate();

private:
    TowerDefenseArena* arena;

    Float3 gridSelection{-1, -1};
    Float3 mouseXZProjection;
    Float3 mouseScreenPosition;

    std::unordered_set<int> keysToTrack{
        VK_LBUTTON, VK_RBUTTON, VK_SPACE, 'A', 'D', 'R'
    };

    std::unordered_map<int, boolean> currentState;
    std::unordered_map<int, boolean> lastState;

    void CalculateGridSelection();
};
