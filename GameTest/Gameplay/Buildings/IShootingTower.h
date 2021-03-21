#pragma once
#include "IBuilding.h"

class IShootingTower : public IBuilding
{
public:
    IShootingTower() : IShootingTower(Float3(0, 0, 1))
    {
    }

    IShootingTower(Float3 baseColor)
        : IBuilding(baseColor)
    {
    }
};
