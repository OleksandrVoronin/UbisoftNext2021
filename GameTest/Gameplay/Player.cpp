#include "stdafx.h"
#include "Player.h"
#include "TowerDefenseArena.h"

void Player::CalculateGridSelection()
{
    float mouseX;
    float mouseY;
    App::GetMousePos(mouseX, mouseY);
    mouseX /= static_cast<float>(WINDOW_WIDTH);
    mouseY /= static_cast<float>(WINDOW_HEIGHT);

    Ray3 screenToWorldRay = arena->GetCamera()->GetRayFromUV(mouseX, mouseY);
    mouseXZProjection = screenToWorldRay.GetPositionAlongRay(screenToWorldRay.Test_MagnitudeToYZero());

    gridSelection = Float3(floor(mouseXZProjection.x / arena->stepX), 0, floor(mouseXZProjection.z / arena->stepZ));
}

void Player::RenderMouseXZProjection() const
{
    const float size = 0.25f;
    const Float3 a = arena->GetCamera()->WorldToCamera(mouseXZProjection + Float3(-size, 0, -size));
    const Float3 b = arena->GetCamera()->WorldToCamera(mouseXZProjection + Float3(-size, 0, size));
    const Float3 c = arena->GetCamera()->WorldToCamera(mouseXZProjection + Float3(size, 0, size));
    const Float3 d = arena->GetCamera()->WorldToCamera(mouseXZProjection + Float3(size, 0, -size));

    arena->GetLineRenderer()->DrawLineSingleColor(a, b, Float3(1, 0, 0));
    arena->GetLineRenderer()->DrawLineSingleColor(b, c, Float3(1, 0, 0));
    arena->GetLineRenderer()->DrawLineSingleColor(c, d, Float3(1, 0, 0));
    arena->GetLineRenderer()->DrawLineSingleColor(d, a, Float3(1, 0, 0));
}

void Player::RenderGridSelection() const
{
    arena->RenderSquare((int) gridSelection.x, (int) gridSelection.z, 0.45f, Float3(1, 0, 0));
}
