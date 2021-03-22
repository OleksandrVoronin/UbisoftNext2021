#pragma once
#include "IEnemy.h"
#include "../../Drawables/DrawableGem.h"

class EnemyBoss : public IEnemy
{
public:
    EnemyBoss(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
              float renderScale, float strengthMultiplier);
    ~EnemyBoss();
    int GetMaxHealthPoints() override;
    void Render(LineRenderer* renderer, Camera* camera) override;
    void Update(float deltaTime) override;

protected:
    float GetMovespeed() override;

private:
    Float3 enemyColor{0, 0, 0};
    DrawableGem* enemyGraphics;
};
