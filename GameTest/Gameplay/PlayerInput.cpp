#include "stdafx.h"
#include "PlayerInput.h"
#include "TowerDefenseArena.h"
#include "../App/app.h"
#include "../App/main.h"
#include "../Math/Ray3.h"


boolean PlayerInput::GetLMousePressedThisFrame() const
{
    return !lastLMousePressedState && currentLMousePressedState;
}

boolean PlayerInput::GetLMouseReleasedThisFrame() const
{
    return lastLMousePressedState && !currentLMousePressedState;
}

boolean PlayerInput::GetRMousePressedThisFrame() const
{
    return !lastRMousePressedState && currentRMousePressedState;
}

boolean PlayerInput::GetRMouseReleasedThisFrame() const
{
    return lastRMousePressedState && !currentRMousePressedState;
}

PlayerInput::PlayerInput(TowerDefenseArena* arena): arena(arena)
{
}

void PlayerInput::Update()
{
    currentLMousePressedState = App::IsKeyPressed(VK_LBUTTON);
    currentRMousePressedState = App::IsKeyPressed(VK_RBUTTON);
    CalculateGridSelection();
}

void PlayerInput::LateUpdate()
{
    lastLMousePressedState = currentLMousePressedState;
    lastRMousePressedState = currentRMousePressedState;
}

void PlayerInput::CalculateGridSelection()
{
    App::GetMousePos(mouseScreenPosition.x, mouseScreenPosition.y);

    Ray3 screenToWorldRay = arena->GetCamera()->GetRayFromUV(
        mouseScreenPosition.x / static_cast<float>(WINDOW_WIDTH),
        mouseScreenPosition.y / static_cast<float>(WINDOW_HEIGHT));
    mouseXZProjection = screenToWorldRay.GetPositionAlongRay(screenToWorldRay.Test_MagnitudeToYZero());

    gridSelection = Float3(floor(mouseXZProjection.x / arena->tileSizeX), 0, floor(mouseXZProjection.z / arena->tileSizeZ));
}
