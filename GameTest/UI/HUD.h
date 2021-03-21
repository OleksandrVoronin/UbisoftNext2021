#pragma once
#include "UIButton.h"
#include "../LineRenderer.h"
#include "../App/app.h"
#include "../App/main.h"
#include "../Gameplay/ArenaTile.h"
#include "../Gameplay/PlayerInput.h"
#include "../Gameplay/PlayerState.h"
#include "../Gameplay/TowerDefenseArena.h"
#include "../Gameplay/Buildings/CoreBuilding.h"
#include "../Gameplay/Buildings/TowerBasic.h"
#include "../Gameplay/Buildings/TowerFrost.h"
#include "../Math/Float3.h"

class TowerDefenseArena;

class HUD
{
public:
    HUD(TowerDefenseArena* arena);

    ~HUD()
    {
        for (int i = 0; i < buildButtons.size(); i++)
        {
            delete buildButtons[i];
        }
        buildButtons.clear();
        delete hudLineRenderer;
    }

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


    void DrawRhombus(Float3 centerAt, float radius, Float3 color, float sizeMultiplier = 1) const
    {
        App::DrawLine(centerAt.x - radius * sizeMultiplier, centerAt.y,
                      centerAt.x, centerAt.y + radius * sizeMultiplier, color.x, color.y, color.z);
        App::DrawLine(centerAt.x, centerAt.y + radius * sizeMultiplier,
                      centerAt.x + radius * sizeMultiplier, centerAt.y, color.x, color.y, color.z);
        App::DrawLine(centerAt.x + radius * sizeMultiplier, centerAt.y,
                      centerAt.x, centerAt.y - radius * sizeMultiplier, color.x, color.y, color.z);
        App::DrawLine(centerAt.x, centerAt.y - radius * sizeMultiplier,
                      centerAt.x - radius * sizeMultiplier, centerAt.y, color.x, color.y, color.z);
    }

    void RenderBuildingInformation(IBuilding* building, boolean includeUpgradeInformation = false) const
    {
        std::string levelAppendedName = building->GetName();
        if (building->GetLevel() > 0)
        {
            levelAppendedName += "+" + std::to_string(building->GetLevel());
            if (!building->IsUpgradeable())
            {
                levelAppendedName += " (max level)";
            }
        }

        App::Print(1200, 970, (levelAppendedName).c_str(),
                   textColor.x, textColor.y, textColor.z,
                   GLUT_BITMAP_TIMES_ROMAN_24);

        for (int i = 0; i < building->GetDescription().size(); i++)
        {
            App::Print(1200, static_cast<float>(940 - 20.0f * i),
                       building->GetDescription()[i].c_str(),
                       textColor.x, textColor.y, textColor.z,
                       GLUT_BITMAP_HELVETICA_18);
        }
    }
};
