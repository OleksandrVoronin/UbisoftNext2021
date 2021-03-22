#include "stdafx.h"
#include "EnemyHeavy.h"

EnemyHeavy::EnemyHeavy(const Float3& position, Transform* worldRoot, std::vector<Float3>* path, float renderScale,
                       float strengthMultiplier): IEnemy(position, worldRoot, path, renderScale, strengthMultiplier)
{
    enemyGraphics = new DrawableRhombus(Float3(), this, 0.5f);
    ResetHealth();
}

EnemyHeavy::~EnemyHeavy()
{
    delete enemyGraphics;
}

int EnemyHeavy::GetMaxHealthPoints()
{
    return static_cast<int>(20.0f * strengthMultiplier);
}

void EnemyHeavy::Render(LineRenderer* renderer, Camera* camera)
{
    enemyGraphics->Render(renderer, camera, enemyColor);
    RenderHealthBar(renderer, camera, Float3(0, renderScale / 4.0f));
}

void EnemyHeavy::Update(float deltaTime)
{
    IWaypointMover::Update(deltaTime);
    enemyGraphics->RotateBy(0, deltaTime, 0);
}

float EnemyHeavy::GetMovespeed()
{
    return 1.3f * strengthMultiplier;
}
