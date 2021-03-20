#include "stdafx.h"
#include "Float3.h"

Float3 Float3::operator+(Float3 other) const
{
    return Float3(x + other.x, y + other.y, z + other.z);    
}

Float3 Float3::operator-(Float3 other) const
{
    return Float3(x - other.x, y - other.y, z - other.z);    
}

Float3 Float3::operator*(float scalar) const
{
    return Float3(x * scalar, y * scalar, z *scalar);    
}

Float3 Float3::operator*(Float3 other) const
{
    return Float3(x * other.x, y * other.y, z * other.z);
}

Float3 Float3::operator-()
{
    return Float3(x * -1, y * -1, z * -1);    
}

float Float3::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

float Float3::LengthSquared() const
{
    return x * x + y * y + z * z;
}

Float3 Float3::DirectionVector(const Float3 from, const Float3 to)
{
    return Float3(to.x - from.x, to.y - from.y, to.z - from.z);
}
