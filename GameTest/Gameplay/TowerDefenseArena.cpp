#include "stdafx.h"
#include "TowerDefenseArena.h"
#include "Player.h"

TowerDefenseArena::TowerDefenseArena(int cellsX, int cellsZ, Float3* map) : cellsX{cellsX}, cellsZ{cellsZ}
{
    ParseMap(map);

    root = new Transform(Float3(0, 0, 0));

    camera = new Camera(root, Float3{worldSizeX / 2, worldSizeZ * 1.5f, -worldSizeZ}, 1.4f);
    camera->RotateAbsolute(QUARTER_PI, 0, 0);
    camera->UpdateTransform();

    lineRenderer = new LineRenderer();

    player = new Player(this);
}

TowerDefenseArena::~TowerDefenseArena()
{
    delete root;
    delete camera;
    delete lineRenderer;
    delete player;
}

void TowerDefenseArena::Update(float deltaTime) const
{
    player->Update();
    root->UpdateTransform();
}

void TowerDefenseArena::Render() const
{
    RenderArena();
    //VisualizePath();

    player->Render();

    lineRenderer->RenderFrame();
}
