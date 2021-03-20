#pragma once
#include <complex>


class Float3
{
public:
    float x;
    float y;
    float z;
    
    Float3() : x{0}, y{0}, z{0} {}
    Float3(const float x, const float y, const float z) : x{x}, y{y}, z{z} {}
    Float3(const float x, const float y) : x{x}, y{y}, z{0} {}

    Float3 operator+(Float3 other) const;
    Float3 operator-(Float3 other) const;
    Float3 operator*(float scalar) const;
    Float3 operator*(Float3 other) const;
    Float3 operator-();
    
    float Length() const;
    float LengthSquared() const;
    Float3 Normalized() const
    {
        const float length = Length();
       
        return Float3(x / length, y/length, z/length);
    }
    
    static Float3 Lerp(Float3 a, Float3 b, float c)
    {
        return Float3(
        a.x + (b.x - a.x) * c,
        a.y + (b.y - a.y) * c,
        a.z + (b.z - a.z) * c
        );
    }

    static Float3 Lerp(const Float3* a, const Float3* b, float c)
    {
        return Float3(
        a->x + (b->x - a->x) * c,
        a->y + (b->y - a->y) * c,
        a->z + (b->z - a->z) * c
        );
    }
    
    static Float3 DirectionVector(const Float3 from, const Float3 to);
};
