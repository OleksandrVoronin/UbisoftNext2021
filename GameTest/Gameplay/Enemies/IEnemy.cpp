#include "stdafx.h"
#include "IEnemy.h"

IEnemy::IEnemy(const Float3& position, Transform* worldRoot, std::vector<Float3>* path, float renderScale,
               float strengthMultiplier): IWaypointMover(position, worldRoot, path, renderScale),
                                          strengthMultiplier{strengthMultiplier}
{
}

boolean IEnemy::IsAlive() const
{
    return healthPoints > 0;
}

void IEnemy::Damage(float damagePoints)
{
    if (this->GetCurrentTile())
    {
        damagePoints *= this->GetCurrentTile()->GetDamageTakenMultiplier();
    }
    healthPoints -= damagePoints;
}

void IEnemy::RenderHealthBar(LineRenderer* renderer, Camera* camera, Float3 offset)
{
    // Redraw with tiny offsets to mimic thicker line.
    const float yStepOffset = renderScale / 150.0f;

    for (int i = 0; i < 3; i++)
    {
        const float healthBarSize = renderScale / 5.0f;

        const Float3 v1 = camera->WorldToCamera(offset + GetWorldPosition() + Float3(-healthBarSize, 0));
        const Float3 v2 = camera->WorldToCamera(offset +
            GetWorldPosition() + Float3(-healthBarSize + GetHealthPercentage() * healthBarSize * 2.f, 0));
        //const Float3 v3 = camera->WorldToCamera(offset + GetWorldPosition() + Float3(+healthBarSize, 0));

        renderer->DrawLineFogApplied(&v1, &v2, healthColor);
        //renderer->DrawLineFogApplied(&v2, &v3, missingHealthColor);

        offset.y += yStepOffset;
        offset.z += yStepOffset;
    }
}

float IEnemy::GetHealthPercentage()
{
    return healthPoints / static_cast<float>(GetMaxHealthPoints());
}

void IEnemy::ResetHealth()
{
    healthPoints = GetMaxHealthPoints();
}

int IEnemy::GetHealthPoints() const
{
    return healthPoints;
}
