#pragma once
#include "IShootingTower.h"
#include "TowerBasic.h"
#include "../../Drawables/DrawableGem.h"

class TowerSplash : public IShootingTower
{
public:
    TowerSplash() : TowerSplash(Float3(1, 0, 1), 1.f)
    {
    }

    TowerSplash(Float3 baseColor, float shotCooldown) : IShootingTower(baseColor, shotCooldown)
    {
        gemStone = new DrawableGem(Float3(), this, 1);
    }

    ~TowerSplash()
    {
        delete gemStone;
    }

    void Render(LineRenderer* renderer, Camera* camera) override
    {
        IBuilding::Render(renderer, camera);
        gemStone->Render(renderer, camera, baseColor);

        if (arena && GetShotDisplayProgress() >= 0 && GetShotDisplayProgress() <= 1)
        {
            for (int i = 0; i < hitLocations.size(); i++)
            {
                RenderShotTrail(hitLocations[i], renderer, camera, baseColor,
                                Float3(0, GetUpgradeLevelScale() * renderScale / 3.0f));
            }
        }
    }

    void Update(float deltaTime) override
    {
        IShootingTower::Update(deltaTime);

        SetScale(GetUpgradeLevelScale());
        gemStone->RotateBy(0, deltaTime * 0.5f, 0);
    }

    boolean TryShoot() override
    {
        target = AcquireTarget();

        if (target != nullptr)
        {
            hitList.clear();
            hitLocations.clear();
            hitList.insert(target);

            // Compile the list of enemies in range (taken from nearby tiles' internal lists)
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    // Discard diagonals
                    if (abs(x) == 1 && abs(y) == 1) continue;

                    ArenaTile* iterationTile = arena->GetTileByIndices(
                        static_cast<int>(target->GetCurrentTile()->GetTileCoordinates().x) + x,
                        static_cast<int>(target->GetCurrentTile()->GetTileCoordinates().z) + y);

                    if (iterationTile)
                    {
                        for (std::unordered_set<IEnemy*>::iterator itr = iterationTile->GetEnemiesOnThisTile()->begin();
                             itr != iterationTile->GetEnemiesOnThisTile()->end(); ++itr)
                        {
                            if (((*itr)->GetWorldPosition() - target->GetWorldPosition()).LengthSquared() <
                                splashRadiusSquared)
                            {
                                hitList.insert((*itr));
                            }
                        }
                    }
                }
            }

            for (std::unordered_set<IEnemy*>::iterator itr = hitList.begin(); itr != hitList.end(); ++
                 itr)
            {
                hitLocations.push_back((*itr)->GetWorldPosition());
                (*itr)->Damage(GetDamage());
            }

            return true;
        }

        return false;
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        gemStone->SetScale(renderScale / 1.8f);
        gemStone->MoveToLocal(Float3(0, renderScale / 6.0f, 0));
    }

    std::string GetName() override
    {
        return "Splash Tower";
    }

    std::vector<std::string> GetDescription() override
    {
        return {
            "A tower powerful enough to damage ",
            "numerous foes in a single shot.",
            "Damage: " + std::to_string(static_cast<int>(GetDamage())) + (IsUpgradeable() ? " (+2 next level)" : ""),
            "Range: " + std::to_string(static_cast<int>(GetRange())) + (IsUpgradeable() ? " (+1 next level)" : ""),
            "Fire rate: Low"
        };
    }

    int GetCost() override
    {
        return 10 + 5 * GetLevel();
    }

    int GetRange() override
    {
        return 1 + 1 * GetLevel();
    }

    float GetDamage() override
    {
        return 3.0f + 2.0f * GetLevel();
    }

    float GetUpgradeLevelScale() const override
    {
        return 0.6f + GetLevel() * 0.4f;
    }

    int GetMaxLevel() const override
    {
        return 1;
    }

protected:
    IDrawable* gemStone;
    float splashRadiusSquared = 1;


    std::unordered_set<IEnemy*> hitList;
    std::vector<Float3> hitLocations;
};
