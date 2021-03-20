#pragma once
#include <dsound.h>

#include "../Math/Float3.h"

class ArenaTile
{
public:
    ArenaTile(Float3 position, boolean partOfThePath)
        : position{position}, partOfThePath{partOfThePath}
    {
    }

    Float3 GetPosition() const
    {
        return position;
    }

    boolean GetIsPartOfThePath() const
    {
        return partOfThePath;
    }

private:
    Float3 position;
    boolean partOfThePath = false;
};
