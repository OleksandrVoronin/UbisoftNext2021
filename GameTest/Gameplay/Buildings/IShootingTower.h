#pragma once
#include "IBuilding.h"
#include "../TowerDefenseArena.h"

class IShootingTower : public IBuilding
{
public:
    IShootingTower(float shotCooldown) : IShootingTower(Float3(0, 0, 1), shotCooldown)
    {
    }

    IShootingTower(Float3 baseColor, float shotCooldown)
        : IBuilding(baseColor), shotCooldown(shotCooldown)
    {
    }

    void Update(float deltaTime) override
    {
        if (arena != nullptr && arena->GetTimeElapsed() > lastShotTimestamp + shotCooldown)
        {
            if (TryShoot())
            {
                lastShotTimestamp = arena->GetTimeElapsed();
            }
        }
    }

    virtual boolean TryShoot() = 0;

    IEnemy* AcquireTarget()
    {
        for (std::unordered_set<ArenaTile*>::iterator itr = tilesInRange.begin(); itr != tilesInRange.end(); ++itr)
        {
            ArenaTile* traversalTile = *itr;
            if (traversalTile && traversalTile->GetEnemiesOnThisTile()->size() > 0)
            {
                return (*traversalTile->GetEnemiesOnThisTile()->begin());
            }
        }

        return nullptr;
    }

    virtual void RenderShotTrail(Float3 shotLocation, LineRenderer* renderer, Camera* camera, Float3 color,
                                 Float3 originOffset = Float3())
    {
        // Render shot
        if (arena)
        {
            Float3 offset = Float3();
            const float yOffsetStep = renderScale / 100.0f;
            const float shotDisplayProgress = GetShotDisplayProgress();

            if (shotDisplayProgress >= 0 && shotDisplayProgress <= 1)
            {
                // Varying line thickness based on the display time [1, 3]
                const float lineThickness = max(1, sin(shotDisplayProgress * PI) * 3);

                for (int i = 0; i < lineThickness; i++)
                {
                    const Float3 v1 = camera->WorldToCamera(shotLocation + offset);
                    const Float3 v2 = camera->WorldToCamera(GetWorldPosition() + offset + originOffset);

                    renderer->DrawLineFogApplied(&v1, &v2, color);
                    offset.y += yOffsetStep;
                }
            }
        }
    }

protected:
    const float shotCooldown = 1.f;
    float shotDisplayDuration = 0.1f;
    float lastShotTimestamp = std::numeric_limits<float>::lowest();

    IEnemy* target = nullptr;
    Float3 shotLocation = Float3();

    float GetShotDisplayProgress() const
    {
        return (arena->GetTimeElapsed() - lastShotTimestamp) / shotDisplayDuration;
    }
};
