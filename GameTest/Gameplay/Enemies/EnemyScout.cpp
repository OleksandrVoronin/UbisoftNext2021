#include "stdafx.h"
#include "EnemyScout.h"

EnemyScout::EnemyScout(const Float3& position, Transform* worldRoot, std::vector<Float3>* path, float renderScale,
                       float strengthMultiplier): IEnemy(position, worldRoot, path, renderScale, strengthMultiplier)
{
    enemyGraphics = new DrawableRhombus(Float3(), this, 0.3f);
    ResetHealth();
}

EnemyScout::~EnemyScout()
{
    delete enemyGraphics;
}

int EnemyScout::GetMaxHealthPoints()
{
    return static_cast<int>(5.0f * strengthMultiplier);
}

void EnemyScout::Render(LineRenderer* renderer, Camera* camera)
{
    enemyGraphics->Render(renderer, camera, enemyColor);
    RenderHealthBar(renderer, camera, Float3(0, renderScale / 4.0f));
}

void EnemyScout::Update(float deltaTime)
{
    IWaypointMover::Update(deltaTime);
    enemyGraphics->RotateBy(0, deltaTime, 0);
}

float EnemyScout::GetMovespeed()
{
    return 4.0f * strengthMultiplier;
}
