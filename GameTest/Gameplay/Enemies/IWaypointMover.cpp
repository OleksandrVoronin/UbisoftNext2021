#include "stdafx.h"
#include "IWaypointMover.h"

#include "../ArenaTile.h"


void IWaypointMover::Update(float deltaTime)
{
    if (path == nullptr || (*path).size() <= currentWayPoint) return;

    // Distance calculations below are using squares for optimization
    const Float3 targetVector = (*path)[currentWayPoint] - GetWorldPosition();
    const float distanceToGoal = (targetVector).LengthSquared();
    if (distanceToGoal < wayPointProximityEpsilon)
    {
        currentWayPoint++;
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
