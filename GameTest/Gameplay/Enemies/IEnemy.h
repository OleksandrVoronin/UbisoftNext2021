#pragma once
#include "IWaypointMover.h"

class IEnemy : public IWaypointMover
{
public:
    IEnemy(const Float3& position, Transform* worldRoot, std::vector<Float3>* path,
           float renderScale, float strengthMultiplier);

    boolean IsAlive() const;
    void Damage(float damagePoints);

protected:
    Float3 healthColor{0, 0.5f, 0};
    //Float3 missingHealthColor{0.5f, 0, 0};

    float strengthMultiplier = 1.0f;
    float healthPoints;

    virtual int GetMaxHealthPoints() = 0;

    void RenderHealthBar(LineRenderer* renderer, Camera* camera, Float3 offset = Float3());
    float GetHealthPercentage();
    void ResetHealth();
    int GetHealthPoints() const;
};
