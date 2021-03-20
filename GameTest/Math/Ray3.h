#pragma once
#include "Float3.h"

class Ray3
{
public:
    Float3 origin;
    Float3 direction;

    Ray3(Float3 origin, Float3 direction);

    void Normalize();
    Float3 GetPositionAlongRay(float distance);
    
    float Test_MagnitudeToYZero()
    {
        if((origin.y >= 0 && direction.y >= 0)
            || (origin.y <= 0 && direction.y <= 0))
        {
            return -1;
        }

        return abs(origin.y / direction.y);
    }
};
