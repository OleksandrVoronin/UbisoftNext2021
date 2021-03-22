#pragma once
#include "UIButton.h"
#include "../Utils/LineRenderer.h"
#include "../Gameplay/ArenaTile.h"
#include "../Gameplay/PlayerInput.h"
#include "../Gameplay/PlayerState.h"
#include "../Gameplay/Buildings/CoreBuilding.h"
#include "../Math/Float3.h"

class TowerDefenseArena;

class HUD
{
public:
    HUD(TowerDefenseArena* arena);
    ~HUD();

    void Update();
    void Render();

private:
    const Float3 creditsColor{0.102f, 0.553f, 0.561f};
    const Float3 livesColor{0.561f, 0.102f, 0.102f};
    const Float3 textColor{0.0f, 0.0f, 0.0f};
    const Float3 textWarningColor{0.9f, 0.0f, 0.0f};
    std::string textBuffer;
    LineRenderer* hudLineRenderer;
    TowerDefenseArena* arena;

    BuildingButton* currentBuildingSelection = nullptr;
    std::vector<BuildingButton*> buildButtons;

    void DrawRhombus(Float3 centerAt, float radius, Float3 color, float sizeMultiplier = 1) const;
    void RenderBuildingInformation(IBuilding* building, boolean includeUpgradeInformation = false) const;
    void RenderInfoLabels();
    void RenderBuildButtons();
    void RenderMapMouseSelection();
    void RenderGrabbedObjects();
    void RenderDeathScreen();
};
