#pragma once
#include "IEnemy.h"

class EnemyCorner : public IEnemy
{
public:
    EnemyCorner(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
                float renderScale, float strengthMultiplier)
        : IEnemy(position, worldRoot, path, renderScale, strengthMultiplier)
    {
        enemyGraphics = new DrawableRhombus(Float3(), this, 0.6f);
        ResetHealth();
    }

    ~EnemyCorner()
    {
        delete enemyGraphics;
    }

    int GetMaxHealthPoints() override
    {
        return static_cast<int>(30.0f * strengthMultiplier);
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
        return 6.0f * strengthMultiplier;
    }

private:
    Float3 enemyColor{0, 0.3, 0.3};
    DrawableRhombus* enemyGraphics;
};
