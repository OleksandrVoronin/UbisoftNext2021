#pragma once
#include "IEnemy.h"

class EnemyHeavy : public IEnemy
{
public:
    EnemyHeavy(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
               float renderScale, float strengthMultiplier)
        : IEnemy(position, worldRoot, path, renderScale, strengthMultiplier)
    {
        enemyGraphics = new DrawableRhombus(Float3(), this, 0.5f);
        ResetHealth();
    }

    ~EnemyHeavy()
    {
        delete enemyGraphics;
    }

    int GetMaxHealthPoints() override
    {
        return static_cast<int>(20.0f * strengthMultiplier);
    }

    void Render(LineRenderer* renderer, Camera* camera) override
    {
        enemyGraphics->Render(renderer, camera, enemyColor);
        RenderHealthBar(renderer, camera, Float3(0, renderScale / 4.0f));
    }

    void Update(float deltaTime) override
    {
        IWaypointMover::Update(deltaTime);
        enemyGraphics->RotateBy(0, deltaTime, 0);
    }

protected:
    float GetMovespeed() override
    {
        return 1.3f * strengthMultiplier;
    }

private:
    Float3 enemyColor{0, 0, 1};
    DrawableRhombus* enemyGraphics;
};
