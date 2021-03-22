#include "stdafx.h"
#include "EnemySoldier.h"

EnemySoldier::EnemySoldier(const Float3& position, Transform* worldRoot, std::vector<Float3>* path, float renderScale,
                           float strengthMultiplier): IEnemy(position, worldRoot, path, renderScale, strengthMultiplier)
{
    enemyGraphics = new DrawableRhombus(Float3(), this, 0.4f);
    ResetHealth();
}

EnemySoldier::~EnemySoldier()
{
    delete enemyGraphics;
}

int EnemySoldier::GetMaxHealthPoints()
{
    return static_cast<int>(10.0f * strengthMultiplier);
}

void EnemySoldier::Render(LineRenderer* renderer, Camera* camera)
{
    enemyGraphics->Render(renderer, camera, enemyColor);
    RenderHealthBar(renderer, camera, Float3(0, renderScale / 4.0f));
}

void EnemySoldier::Update(float deltaTime)
{
    IWaypointMover::Update(deltaTime);
    enemyGraphics->RotateBy(0, deltaTime, 0);
}

float EnemySoldier::GetMovespeed()
{
    return 2.0f * strengthMultiplier;
}
