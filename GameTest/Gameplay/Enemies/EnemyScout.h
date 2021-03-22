#pragma once
#include "IEnemy.h"
#include "../../Math/Float3.h"

class EnemyScout : public IEnemy
{
public:
    EnemyScout(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
               float renderScale, float strengthMultiplier)
        : IEnemy(position, worldRoot, path, renderScale, strengthMultiplier)
    {
        enemyGraphics = new DrawableRhombus(Float3(), this, 0.3f);
        ResetHealth();
    }

    ~EnemyScout()
    {
        delete enemyGraphics;
    }

    int GetMaxHealthPoints() override
    {
        return static_cast<int>(5.0f * strengthMultiplier);
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
        return 4.0f * strengthMultiplier;
    }

private:
    Float3 enemyColor{0, 1, 0};
    DrawableRhombus* enemyGraphics;
};
