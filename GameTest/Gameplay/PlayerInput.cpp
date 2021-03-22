#include "stdafx.h"
#include "PlayerInput.h"
#include "TowerDefenseArena.h"
#include "../App/app.h"
#include "../App/main.h"
#include "../Math/Ray3.h"

PlayerInput::PlayerInput(TowerDefenseArena* arena): arena(arena)
{
    for (std::unordered_set<int>::iterator itr = keysToTrack.begin(); itr != keysToTrack.end();
         ++itr)
    {
        currentState[*itr] = false;
        lastState[*itr] = false;
    }
}

boolean PlayerInput::GetKeyPressedThisFrame(int keycode)
{
    if (keysToTrack.count(keycode) == 0)
    {
        return false;
    }

    return !lastState[keycode] && currentState[keycode];
}

boolean PlayerInput::GetKeyReleasedThisFrame(int keycode)
{
    if (keysToTrack.count(keycode) == 0)
    {
        return false;
    }

    return lastState[keycode] && !currentState[keycode];
}

void PlayerInput::Update()
{
    for (std::unordered_set<int>::iterator itr = keysToTrack.begin(); itr != keysToTrack.end();
         ++itr)
    {
        currentState[*itr] = App::IsKeyPressed(*itr);
    }

    CalculateGridSelection();
}

void PlayerInput::LateUpdate()
{
    for (std::unordered_set<int>::iterator itr = keysToTrack.begin(); itr != keysToTrack.end();
         ++itr)
    {
        lastState[*itr] = currentState[*itr];
    }
}

void PlayerInput::CalculateGridSelection()
{
    App::GetMousePos(mouseScreenPosition.x, mouseScreenPosition.y);

    Ray3 screenToWorldRay = arena->GetCamera()->GetRayFromUV(
        mouseScreenPosition.x / static_cast<float>(WINDOW_WIDTH),
        mouseScreenPosition.y / static_cast<float>(WINDOW_HEIGHT));
    mouseXZProjection = screenToWorldRay.GetPositionAlongRay(screenToWorldRay.Test_MagnitudeToYZero());

    gridSelection = Float3(floor(mouseXZProjection.x / arena->tileSizeX), 0,
                           floor(mouseXZProjection.z / arena->tileSizeZ));
}
