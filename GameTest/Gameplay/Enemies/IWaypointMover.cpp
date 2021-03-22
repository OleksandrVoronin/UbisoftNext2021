#include "stdafx.h"
#include "IWaypointMover.h"

#include "../ArenaTile.h"


IWaypointMover::IWaypointMover(Float3 position, Transform* worldRoot, std::vector<Float3>* path, float renderScale):
    Transform(
        position, worldRoot), renderScale{renderScale},
    path{path}
{
    SetScale(renderScale);
}

void IWaypointMover::Update(float deltaTime)
{
    if (path == nullptr || (*path).size() <= currentWayPoint) return;

    if (idleTimeLeft > 0)
    {
        idleTimeLeft -= deltaTime;
        return;
    }

    // Distance calculations below are using squares for optimization
    const Float3 targetVector = (*path)[currentWayPoint] - GetWorldPosition();
    const float distanceToGoal = (targetVector).LengthSquared();
    if (distanceToGoal < wayPointProximityEpsilon)
    {
        currentWayPoint++;
        idleTimeLeft = waypointSwitchDelay;
    }
    else
    {
        float moveSpeed = this->GetMovespeed() * renderScale;
        if (this->GetCurrentTile())
        {
            moveSpeed *= this->GetCurrentTile()->GetSpeedMultiplier();
        }

        if (moveSpeed * moveSpeed * deltaTime * deltaTime > distanceToGoal)
        {
            SetWorldPosition((*path)[currentWayPoint]);
        }
        else
        {
            MoveByLocal(targetVector.Normalized() * (moveSpeed * deltaTime));
        }
    }
}

ArenaTile* IWaypointMover::GetCurrentTile() const
{
    return currentTile;
}

void IWaypointMover::SetCurrentTile(ArenaTile* currentTile)
{
    this->currentTile = currentTile;
}
