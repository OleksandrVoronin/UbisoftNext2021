#pragma once
#include "IEnemy.h"

class EnemySoldier : public IEnemy
{
public:
    EnemySoldier(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
                 float renderScale, float strengthMultiplier);

    ~EnemySoldier();

    int GetMaxHealthPoints() override;
    void Render(LineRenderer* renderer, Camera* camera) override;
    void Update(float deltaTime) override;

protected:
    float GetMovespeed() override;

private:
    Float3 enemyColor{1, 0, 0};
    DrawableRhombus* enemyGraphics;
};
