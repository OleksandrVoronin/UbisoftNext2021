#pragma once
#include "IEnemy.h"
#include "../../Math/Float3.h"

class EnemyScout : public IEnemy
{
public:
    EnemyScout(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
               float renderScale, float strengthMultiplier);

    ~EnemyScout();

    int GetMaxHealthPoints() override;
    void Render(LineRenderer* renderer, Camera* camera) override;
    void Update(float deltaTime) override;

protected:
    float GetMovespeed() override;

private:
    Float3 enemyColor{0, 1, 0};
    DrawableRhombus* enemyGraphics;
};
