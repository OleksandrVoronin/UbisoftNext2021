#include "stdafx.h"
#include "EnemyBoss.h"

EnemyBoss::EnemyBoss(const Float3& position, Transform* worldRoot, std::vector<Float3>* path, float renderScale,
                     float strengthMultiplier): IEnemy(position, worldRoot, path, renderScale, strengthMultiplier)
{
    enemyGraphics = new DrawableGem(Float3(), this, 0.3f);
    ResetHealth();
}

EnemyBoss::~EnemyBoss()
{
    delete enemyGraphics;
}

int EnemyBoss::GetMaxHealthPoints()
{
    return static_cast<int>(200.0f * strengthMultiplier);
}

void EnemyBoss::Render(LineRenderer* renderer, Camera* camera)
{
    enemyGraphics->Render(renderer, camera, enemyColor);
    RenderHealthBar(renderer, camera, Float3(0, renderScale / 4.0f));
}

void EnemyBoss::Update(float deltaTime)
{
    IWaypointMover::Update(deltaTime);
    enemyGraphics->RotateBy(0, deltaTime, 0);
}

float EnemyBoss::GetMovespeed()
{
    return 1.5f * strengthMultiplier;
}
