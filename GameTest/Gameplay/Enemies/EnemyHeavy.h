#pragma once
#include "IEnemy.h"

class EnemyHeavy : public IEnemy
{
public:
    EnemyHeavy(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
               float renderScale, float strengthMultiplier);
    ~EnemyHeavy();
    int GetMaxHealthPoints() override;
    void Render(LineRenderer* renderer, Camera* camera) override;
    void Update(float deltaTime) override;

protected:
    float GetMovespeed() override;

private:
    Float3 enemyColor{0, 0, 1};
    DrawableRhombus* enemyGraphics;
};
