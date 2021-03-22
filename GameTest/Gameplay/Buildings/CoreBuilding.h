#pragma once
#include "IBuilding.h"
#include "../ArenaTile.h"
#include "../TowerDefenseArena.h"
#include "../../Utils/Camera.h"
#include "../../Utils/LineRenderer.h"
#include "../../Drawables/DrawablePyramid.h"

class CoreBuilding : public IBuilding
{
public:
    CoreBuilding() : CoreBuilding(Float3(0, 0.7f, 0))
    {
    }

    CoreBuilding(Float3 baseColor)
        : IBuilding(baseColor), renderColor(baseColor)
    {
        pyramid = new DrawablePyramid(Float3(), this, 1);
    }

    ~CoreBuilding()
    {
        delete pyramid;
    }

    void Render(LineRenderer* renderer, Camera* camera) override
    {
        IBuilding::Render(renderer, camera);
        pyramid->Render(renderer, camera, renderColor);
    }

    void Update(float deltaTime) override
    {
        const float elapsedTime = arena->GetTimeElapsed();

        pyramid->MoveByLocal(Float3(0, cos(elapsedTime / 2.0f) * renderScale * 0.001f, 0));

        if (tile != nullptr)
        {
            std::unordered_set<IEnemy*>* enemies = tile->GetEnemiesOnThisTile();
            while (enemies->size() > 0)
            {
                arena->DeleteEnemy(*(enemies->begin()));
                arena->GetPlayerState()->DeltaLives(-1);
                lastDamageTakenTimeStamp = elapsedTime;
            }
        }

        if (elapsedTime < lastDamageTakenTimeStamp + damageTakenEffectDuration)
        {
            // Yoyo'ed elapsed time (0..1..0)
            float effectTimeElapsedYoyo = (elapsedTime - lastDamageTakenTimeStamp) / damageTakenEffectDuration;
            effectTimeElapsedYoyo = sin(PI * effectTimeElapsedYoyo);
            renderColor = Float3::Lerp(baseColor, damageColor, (effectTimeElapsedYoyo));

            pyramid->SetScale(renderScale / 2.0f - renderScale / 10.0f * effectTimeElapsedYoyo);
        }
        else
        {
            renderColor = baseColor;
        }
    }

    void SetRenderScale(float renderScale) override
    {
        IBuilding::SetRenderScale(renderScale);
        pyramid->SetScale(renderScale / 2.0f);
        pyramid->MoveToLocal(Float3(0, renderScale / 3.0f, 0));
    }

    std::string GetName() override
    {
        return "Core building";
    }

    std::vector<std::string> GetDescription() override
    {
        return {
            "This is your core building.",
            "Don't let enemies get to it.",
            "Protect at all cost."
        };
    }

    int GetCost() override
    {
        return 0;
    }

    int GetRange() override
    {
        return 0;
    }

    float GetDamage() override
    {
        return 0;
    }

    float GetUpgradeLevelScale() const override
    {
        return 1.0f;
    }

    int GetMaxLevel() const override
    {
        return 0;
    }

private:
    DrawablePyramid* pyramid;

    const float damageTakenEffectDuration = 0.3f;
    const Float3 damageColor{1, 0, 0};

    float lastDamageTakenTimeStamp = std::numeric_limits<float>::lowest();
    Float3 renderColor;
};
