#include "stdafx.h"
#include "ArenaTile.h"
#include "Buildings/IBuilding.h"
#include "Enemies/IEnemy.h"

std::unordered_set<IEnemy*>* ArenaTile::GetEnemiesOnThisTile()
{
    return &enemiesAtThisTile;
}

void ArenaTile::RegisterBuilding(IBuilding* building)
{
    this->building = building;
    this->building->SetTile(this);
}

float ArenaTile::GetSpeedMultiplier() const
{
    return speedMultiplier;
}

float ArenaTile::GetDamageTakenMultiplier() const
{
    return damageTakenMultiplier;
}

void ArenaTile::SetSpeedMultiplier(float speedMultiplier)
{
    this->speedMultiplier = speedMultiplier;
}

void ArenaTile::SetDamageTakenMultiplier(float damageTakenMultiplier)
{
    this->damageTakenMultiplier = damageTakenMultiplier;
}
