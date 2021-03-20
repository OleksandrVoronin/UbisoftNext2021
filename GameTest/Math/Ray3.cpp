#include "stdafx.h"
#include "Ray3.h"

Ray3::Ray3(Float3 origin, Float3 direction)
{
    this->origin = origin;
    this->direction = direction;
    
    Normalize();
}

void Ray3::Normalize()
{
    direction = direction.Normalized();
}


Float3 Ray3::GetPositionAlongRay(float distance)
{
    return Float3(origin + direction * distance); 
}
