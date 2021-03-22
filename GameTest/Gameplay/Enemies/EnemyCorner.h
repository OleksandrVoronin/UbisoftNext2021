#pragma once
#include "IEnemy.h"

class EnemyCorner : public IEnemy
{
public:
    EnemyCorner(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
                float renderScale, float strengthMultiplier);

    ~EnemyCorner();

    int GetMaxHealthPoints() override;

    void Render(LineRenderer* renderer, Camera* camera) override;
    void Update(float deltaTime) override;

protected:
    float GetMovespeed() override;

private:
    Float3 enemyColor{0.0f, 0.3f, 0.3f};
    DrawableRhombus* enemyGraphics;
};
