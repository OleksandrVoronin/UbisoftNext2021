#pragma once
#include "../ArenaTile.h"
#include "../../Drawables/DrawableRhombus.h"
#include "../../Utils/Transform.h"


class ArenaTile;

class IWaypointMover : public Transform
{
public:
    IWaypointMover(Float3 position, Transform* worldRoot, std::vector<Float3>* path,
                   float renderScale) : Transform(
                                            position, worldRoot), renderScale{renderScale},
                                        path{path}
    {
        SetScale(renderScale);
    }

    virtual void Update(float deltaTime);

    virtual void Render(LineRenderer* renderer, Camera* camera) = 0;

    ArenaTile* GetCurrentTile() const;

    void SetCurrentTile(ArenaTile* currentTile);

protected:
    int currentWayPoint = 0;
    float wayPointProximityEpsilon = 0.01f;
    float renderScale = 1.0f;

    std::vector<Float3>* path;
    ArenaTile* currentTile = nullptr;

    virtual float GetMovespeed() = 0;
};
