#include "stdafx.h"
#include "TowerDefenseArena.h"
#include "PlayerInput.h"
#include "../UI/HUD.h"

TowerDefenseArena::TowerDefenseArena(int cellsX, int cellsZ, std::vector<Float3> map) : cellCountX{cellsX},
    cellCountZ{cellsZ}
{
    playerState = new PlayerState();

    worldRoot = new Transform(Float3(0, 0, 0));

    ParseMap(map);

    camera = new Camera(worldRoot, Float3{arenaSizeX / 2, arenaSizeZ * 1.5f, -arenaSizeZ}, 1.4f);
    camera->RotateAbsolute(QUARTER_PI, 0, 0);
    camera->UpdateTransform();

    lineRenderer = new LineRenderer(Float3(1, 1, 1));

    playerInput = new PlayerInput(this);

    hud = new HUD(this);
}

TowerDefenseArena::~TowerDefenseArena()
{
    delete camera;
    delete lineRenderer;
    delete playerInput;
    delete playerState;
    delete worldRoot;
}

void TowerDefenseArena::Update(float deltaTime) const
{
    playerInput->Update();
    hud->Update();

    for (std::unordered_set<IBuilding*>::iterator itr = buildings.begin(); itr != buildings.end(); ++itr)
    {
        (*itr)->Update(deltaTime);
    }

    worldRoot->UpdateTransform();

    playerInput->LateUpdate();
}

void TowerDefenseArena::Render() const
{
    RenderArena();
    //VisualizePath();

    for (std::unordered_set<IBuilding*>::iterator itr = buildings.begin(); itr != buildings.end(); ++itr)
    {
        (*itr)->Render(lineRenderer, camera);
    }

    lineRenderer->RenderFrame();

    // HUD needs to render as a second pass, on top of the game
    hud->Render();
}
