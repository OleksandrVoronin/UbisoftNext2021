#pragma once
#include <dsound.h>

#include "../Math/Float3.h"
#include "Buildings/IBuilding.h"

class ArenaTile
{
public:
    ArenaTile(Float3 position, boolean partOfThePath)
        : position{position}, partOfThePath{partOfThePath}
    {
    }

    Float3 GetTileCoordinates() const
    {
        return position;
    }

    boolean GetIsPartOfThePath() const
    {
        return partOfThePath;
    }

    IBuilding* GetBuilding() const
    {
        return building;
    }

    void AddBuilding(IBuilding* building)
    {
        this->building = building;
    }

private:
    Float3 position;
    boolean partOfThePath = false;
    IBuilding* building = nullptr;
};
