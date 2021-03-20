#include "stdafx.h"
#include "TowerDefenseArena.h"
#include "Player.h"

TowerDefenseArena::TowerDefenseArena(int cellsX, int cellsZ, std::vector<Float3> map) : cellsX{cellsX}, cellsZ{cellsZ}
{
    root = new Transform(Float3(0, 0, 0));

    ParseMap(map);

    camera = new Camera(root, Float3{worldSizeX / 2, worldSizeZ * 1.5f, -worldSizeZ}, 1.4f);
    camera->RotateAbsolute(QUARTER_PI, 0, 0);
    camera->UpdateTransform();

    lineRenderer = new LineRenderer(Float3(1, 1, 1));

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

    for (int i = 0; i < buildings.size(); i++)
    {
        buildings[i]->Render(lineRenderer, camera);
    }

    player->Render();

    lineRenderer->RenderFrame();
}
