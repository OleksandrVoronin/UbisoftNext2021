#include "stdafx.h"
#include "HUD.h"
#include "../Gameplay/TowerDefenseArena.h"
#include "../Gameplay/WaveChoreographer.h"
#include "../Gameplay/Buildings/TowerSplash.h"
#include "../Gameplay/Buildings/TowerBurst.h"
#include "../Gameplay/Buildings/TowerDamageIncrease.h"
#include "../Utils/LineRenderer.h"

HUD::HUD(TowerDefenseArena* arena): arena{arena}
{
    hudLineRenderer = new LineRenderer(200, 250);

    const float renderScale = min(arena->tileSizeX, arena->tileSizeZ);
    buildButtons.push_back(
        new TypedBuildButton<TowerBasic>(Float3(800 - 280, 920), Float3(120, 140), renderScale));
    buildButtons.push_back(
        new TypedBuildButton<TowerBurst>(Float3(800 - 140, 920), Float3(120, 140), renderScale));
    buildButtons.push_back(
        new TypedBuildButton<TowerSplash>(Float3(800, 920), Float3(120, 140), renderScale));
    buildButtons.push_back(
        new TypedBuildButton<TowerFrost>(Float3(800 + 140, 920), Float3(120, 140), renderScale));
    buildButtons.push_back(
        new TypedBuildButton<TowerDamageIncrease>(Float3(800 + 280, 920), Float3(120, 140), renderScale));
}

void HUD::Update()
{
    if (!arena->GetPlayerState()->IsAlive())
    {
        return;
    }

    const Float3 mouseTileCoordinates = arena->GetPlayerInput()->GetGridSelection();
    ArenaTile* mouseHoverTile = arena->GetTileByIndices((int)mouseTileCoordinates.x, (int)mouseTileCoordinates.z);

    // Placed building interactions
    if (currentBuildingSelection == nullptr && mouseHoverTile != nullptr && mouseHoverTile->GetBuilding())
    {
        if (arena->GetPlayerInput()->GetKeyPressedThisFrame(VK_LBUTTON))
        {
            if (arena->GetPlayerState()->GetCredits() >= mouseHoverTile->GetBuilding()->GetCost())
            {
                arena->GetPlayerState()->DeltaCredits(-mouseHoverTile->GetBuilding()->GetCost());
                mouseHoverTile->GetBuilding()->Upgrade();
            }
        }

        if (arena->GetPlayerInput()->GetKeyPressedThisFrame(VK_RBUTTON))
        {
            arena->GetPlayerState()->DeltaCredits(mouseHoverTile->GetBuilding()->GetSellPrice());
            arena->RemoveBuilding(static_cast<int>(mouseHoverTile->GetTileCoordinates().x),
                                  static_cast<int>(mouseHoverTile->GetTileCoordinates().z));
        }
    }

    // Mouse was released
    if (arena->GetPlayerInput()->GetKeyReleasedThisFrame(VK_LBUTTON))
    {
        if (currentBuildingSelection)
        {
            // Check if building placement is valid, if so -> place.
            if (mouseHoverTile != nullptr && mouseHoverTile->ValidForBuildingPlacement())
            {
                arena->GetPlayerState()->DeltaCredits(-currentBuildingSelection->GetPrototype()->GetCost());
                arena->AddBuilding((int)mouseTileCoordinates.x, (int)mouseTileCoordinates.z,
                                   currentBuildingSelection->GetBuilding());
            }
        }
        currentBuildingSelection = nullptr;
    }

    for (int i = 0; i < buildButtons.size(); i++)
    {
        const boolean mouseOver = buildButtons[i]->PixelInBounds(arena->GetPlayerInput()->GetMouseScreenPosition());

        if (currentBuildingSelection == nullptr)
        {
            buildButtons[i]->SetState(mouseOver ? UIButton::Hovered : UIButton::Enabled);
        }

        buildButtons[i]->SetState((currentBuildingSelection == buildButtons[i])
                                      ? UIButton::Selected
                                      : buildButtons[i]->GetState());

        const boolean enoughMoney = buildButtons[i]->GetPrototype()->GetCost() <= arena->GetPlayerState()->
            GetCredits();
        buildButtons[i]->SetState(!enoughMoney ? UIButton::Disabled : buildButtons[i]->GetState());

        // Mouse was pressed
        if (mouseOver && enoughMoney && arena->GetPlayerInput()->GetKeyPressedThisFrame(VK_LBUTTON))
        {
            currentBuildingSelection = buildButtons[i];
        }
    }
}

void HUD::Render()
{
    RenderInfoLabels();
    RenderBuildButtons();    
    RenderMapMouseSelection();
    RenderGrabbedObjects();

    if(arena->GetPlayerState()->IsAlive())
    {
        
    }
    
    hudLineRenderer->RenderFrame();
}

void HUD::RenderInfoLabels()
{
    DrawRhombus(Float3(35, 970), 15, creditsColor);
    DrawRhombus(Float3(35, 970), 15, creditsColor, 0.95f);
    textBuffer = std::to_string(arena->GetPlayerState()->GetCredits()) + " credits";
    App::Print(65, 962, textBuffer.c_str(), creditsColor.x, creditsColor.y, creditsColor.z,
               GLUT_BITMAP_TIMES_ROMAN_24);

    DrawRhombus(Float3(35, 925), 15, livesColor);
    DrawRhombus(Float3(35, 925), 15, livesColor, 0.95f);
    textBuffer = std::to_string(arena->GetPlayerState()->GetLives()) + " lives";
    App::Print(65, 917, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z, GLUT_BITMAP_TIMES_ROMAN_24);


    textBuffer = std::to_string(static_cast<int>(arena->GetTimeScale())) + "x speed.";
    App::Print(1485, 50, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z, GLUT_BITMAP_TIMES_ROMAN_24);
    textBuffer = "[A]/[D] to change speed.";
    App::Print(1380, 20, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z, GLUT_BITMAP_HELVETICA_18);


    if (arena->GetWaveChoreographer()->GetIsWaveInProgress())
    {
        textBuffer = "Wave " + std::to_string(arena->GetWaveChoreographer()->GetWaveNumber()) + " in progress..";
        App::Print(20, 850, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z,
                   GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else
    {
        textBuffer = "Wave " + std::to_string(arena->GetWaveChoreographer()->GetWaveNumber() + 1) + " starts in " +
            std::to_string(static_cast<int>(arena->GetWaveChoreographer()->GetTimeUntilNextWave()));
        App::Print(20, 850, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z,
                   GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

void HUD::RenderBuildButtons()
{
    for (int i = 0; i < buildButtons.size(); i++)
    {
        buildButtons[i]->Render();
        if (buildButtons[i]->GetState() == UIButton::Hovered)
        {
            this->RenderBuildingInformation(buildButtons[i]->GetPrototype());
        }

        if (buildButtons[i]->GetState() != UIButton::Selected)
        {
            buildButtons[i]->RenderPreviews(hudLineRenderer, arena->GetCamera());
        }
    }
}

void HUD::RenderMapMouseSelection()
{
    if (currentBuildingSelection == nullptr)
    {
        const Float3 tileCoordinates = arena->GetPlayerInput()->GetGridSelection();
        ArenaTile* tile = arena->GetTileByIndices((int)tileCoordinates.x, (int)tileCoordinates.z);
        if (tile != nullptr && tile->GetBuilding() != nullptr)
        {
            // Regular buildings are never part of the path and are interactable.
            if (!tile->GetIsPartOfThePath())
            {
                tile->GetBuilding()->RenderRange(hudLineRenderer, arena->GetCamera());

                if (tile->GetBuilding()->IsUpgradeable())
                {
                    textBuffer = "[LMB to upgrade for " + std::to_string(tile->GetBuilding()->GetCost()) +
                        "/RMB to sell for " +
                        std::to_string(
                            tile->GetBuilding()->GetSellPrice()) + "]";
                    App::Print(620, 820, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z,
                               GLUT_BITMAP_HELVETICA_18);
                    if (tile->GetBuilding()->GetCost() > arena->GetPlayerState()->GetCredits())
                    {
                        textBuffer = std::to_string(tile->GetBuilding()->GetCost());
                        App::Print(792, 820, textBuffer.c_str(), textWarningColor.x, textWarningColor.y,
                                   textWarningColor.z,
                                   GLUT_BITMAP_HELVETICA_18);
                    }
                }
                else
                {
                    textBuffer = "[RMB to sell for " +
                        std::to_string(
                            tile->GetBuilding()->GetSellPrice()) + "]";
                    App::Print(720, 820, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z,
                               GLUT_BITMAP_HELVETICA_18);
                }
            }

            this->RenderBuildingInformation(tile->GetBuilding());
        }
    }
}

void HUD::RenderGrabbedObjects()
{
    // Render grabbed objects
    if (currentBuildingSelection != nullptr)
    {
        const Float3 dropOffCoordinates = arena->GetPlayerInput()->GetGridSelection();
        ArenaTile* dropOffTile = arena->GetTileByIndices((int)dropOffCoordinates.x, (int)dropOffCoordinates.z);
        if (dropOffTile != nullptr && dropOffTile->ValidForBuildingPlacement())
        {
            // Valid tile
            currentBuildingSelection->GetPrototype()->MoveToLocal(
                arena->GetTilePosition((int)dropOffCoordinates.x, (int)dropOffCoordinates.z) + Float3(
                    arena->tileSizeX / 2, 0, arena->tileSizeZ / 2));

            textBuffer = "[Release LMB to place]";
            App::Print(710, 820, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z,
                       GLUT_BITMAP_HELVETICA_18);

            currentBuildingSelection->GetPrototype()->UpdateTransform();
            currentBuildingSelection->GetPrototype()->RenderRange(hudLineRenderer, arena->GetCamera());
        }
        else
        {
            // Invalid tile
            currentBuildingSelection->GetPrototype()->MoveToLocal(arena->GetPlayerInput()->GetXZProjection());
            textBuffer = "[Choose a valid tile to place / release LMB to dismiss]";
            App::Print(585, 820, textBuffer.c_str(), livesColor.x, livesColor.y, livesColor.z,
                       GLUT_BITMAP_HELVETICA_18);
        }

        currentBuildingSelection->GetPrototype()->UpdateTransform();
        currentBuildingSelection->GetPrototype()->Render(hudLineRenderer, arena->GetCamera());
    }
}
