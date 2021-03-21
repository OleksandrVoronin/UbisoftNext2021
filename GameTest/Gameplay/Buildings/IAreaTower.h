#pragma once
#include "IBuilding.h"

class IAreaTower : public IBuilding
{
public:
    IAreaTower() : IAreaTower(Float3(0, 0, 1))
    {
    }

    IAreaTower(Float3 baseColor)
        : IBuilding(baseColor)
    {
    }
};
