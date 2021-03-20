#pragma once
#include "Float3.h"

class Matrix3
{
public:
    float entries[3][3];

    float* operator[] (int row)
    {
        return entries[row];
    }

    const float* operator [] (int Row) const
    {
        return entries[Row];
    }

    Float3 Matrix3::operator * (const Float3 vector) const
    {
        Float3 result;
        result.x = entries[0][0] * vector.x + entries[0][1] * vector.y + entries[0][2] * vector.z;
        result.y = entries[1][0] * vector.x + entries[1][1] * vector.y + entries[1][2] * vector.z;
        result.z = entries[2][0] * vector.x + entries[2][1] * vector.y + entries[2][2] * vector.z;

        return result;
    }

    Matrix3 Matrix3::operator * (const Matrix3 other) const
    {
        Matrix3 result;

        result[0][0] = entries[0][0] * other[0][0] + entries[1][0] * other[0][1] + entries[2][0] * other[0][2];
        result[1][0] = entries[0][0] * other[1][0] + entries[1][0] * other[1][1] + entries[2][0] * other[1][2];
        result[2][0] = entries[0][0] * other[2][0] + entries[1][0] * other[2][1] + entries[2][0] * other[2][2];

        result[0][1] = entries[0][1] * other[0][0] + entries[1][1] * other[0][1] + entries[2][1] * other[0][2];
        result[1][1] = entries[0][1] * other[1][0] + entries[1][1] * other[1][1] + entries[2][1] * other[1][2];
        result[2][1] = entries[0][1] * other[2][0] + entries[1][1] * other[2][1] + entries[2][1] * other[2][2];

        result[0][2] = entries[0][2] * other[0][0] + entries[1][2] * other[0][1] + entries[2][2] * other[0][2];
        result[1][2] = entries[0][2] * other[1][0] + entries[1][2] * other[1][1] + entries[2][2] * other[1][2];
        result[2][2] = entries[0][2] * other[2][0] + entries[1][2] * other[2][1] + entries[2][2] * other[2][2];
        
        return result;
    }

    Matrix3() : entries{}
    {
    }

    Matrix3(const Float3 a, const Float3 b, const Float3 c)
    {
        entries[0][0] = a.x;
        entries[0][1] = a.y;
        entries[0][2] = a.z;

        entries[1][0] = b.x;
        entries[1][1] = b.y;
        entries[1][2] = b.z;
        
        entries[2][0] = c.x;
        entries[2][1] = c.y;
        entries[2][2] = c.z;
    }

    

    Matrix3 Inverse() const
    {
        Matrix3 result;
        const float a11 = entries[1][1] * entries[2][2] - entries[1][2] * entries[2][1];
        const float a12 = -(entries[0][1] * entries[2][2] - entries[0][2] * entries[2][1]);
        const float a13 = entries[0][1] * entries[1][2] - entries[0][2] * entries[1][1];

        const float determinant = entries[0][0] * a11 + entries[1][0] * a12 + entries[2][0] * a13;
        const float oneOverDeterminant = 1.0f/determinant;

        const float a21 = -(entries[1][0] * entries[2][2] - entries[1][2] * entries[2][0]);
        const float a22 = entries[0][0] * entries[2][2] - entries[0][2] * entries[2][0];
        const float a23 = -(entries[0][0] * entries[1][2] - entries[0][2] * entries[1][0]);
        const float a31 = entries[1][0] * entries[2][1] - entries[1][1] * entries[2][0];
        const float a32 = -(entries[2][1] * entries[0][0] - entries[0][1] * entries[2][0]);
        const float a33 = entries[0][0] * entries[1][1] - entries[0][1] * entries[1][0];

        result[0][0] = oneOverDeterminant * a11;
        result[0][1] = oneOverDeterminant * a12;
        result[0][2] = oneOverDeterminant * a13;
        result[1][0] = oneOverDeterminant * a21;
        result[1][1] = oneOverDeterminant * a22;
        result[1][2] = oneOverDeterminant * a23;
        result[2][0] = oneOverDeterminant * a31;
        result[2][1] = oneOverDeterminant * a32;
        result[2][2] = oneOverDeterminant * a33;

        return result;
    }

    static Matrix3 CombinedRotationMatrix(float xRotation, float yRotation, float zRotation)
    {
        Matrix3 result;

        result[0][0] = cos(zRotation) * cos(yRotation);
        result[0][1] = sin(zRotation) * cos(yRotation);
        result[0][2] = -sin(yRotation);

        result[1][0] = cos(zRotation) * sin(yRotation) * sin(xRotation) - sin(zRotation) * cos(xRotation);
        result[1][1] = sin(zRotation) * sin(yRotation) * sin(xRotation) + cos(zRotation) * cos(xRotation);
        result[1][2] = cos(yRotation) * sin(xRotation);

        result[2][0] = cos(zRotation) * sin(yRotation) * cos(xRotation) + sin(zRotation) * sin(xRotation);
        result[2][1] = sin(zRotation) * sin(yRotation) * cos(xRotation) - cos(zRotation) * sin(xRotation);
        result[2][2] = cos(yRotation) * cos(xRotation);

        return result;
    }
};
