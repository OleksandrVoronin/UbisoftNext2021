#include "stdafx.h"
#include "EnemyCorner.h"

EnemyCorner::EnemyCorner(const Float3& position, Transform* worldRoot, std::vector<Float3>* path, float renderScale,
                         float strengthMultiplier): IEnemy(position, worldRoot, path, renderScale, strengthMultiplier)
{
    enemyGraphics = new DrawableRhombus(Float3(), this, 0.6f);
    this->waypointSwitchDelay = 1.0f;
    ResetHealth();
}

EnemyCorner::~EnemyCorner()
{
    delete enemyGraphics;
}

int EnemyCorner::GetMaxHealthPoints()
{
    return static_cast<int>(30.0f * strengthMultiplier);
}

void EnemyCorner::Render(LineRenderer* renderer, Camera* camera)
{
    enemyGraphics->Render(renderer, camera, enemyColor);
    RenderHealthBar(renderer, camera, Float3(0, renderScale / 4.0f));
}

void EnemyCorner::Update(float deltaTime)
{
    IWaypointMover::Update(deltaTime);
    enemyGraphics->RotateBy(0, deltaTime, 0);
}

float EnemyCorner::GetMovespeed()
{
    return 6.0f * strengthMultiplier;
}
