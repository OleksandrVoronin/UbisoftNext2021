#pragma once
#include "Transform.h"
#include "../App/app.h"
#include "../App/main.h"
#include "../Math/Float3.h"
#include "../Math/Matrix3.h"
#include "../Math/Ray3.h"

class Camera : public Transform
{
public:

    float focal_length;

    Camera(Transform* parent, Float3 position, float focal_length) : Transform(position, parent),
                                                  focal_length{focal_length},
                                                  aspect_ratio{WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT)},
                                                  hAperture{wAperture / aspect_ratio}
    {
        // Cache some values
        wHalfScreen = WINDOW_WIDTH / 2.0f;
        hHalfScreen = WINDOW_HEIGHT / 2.0f;
        wPixels = (WINDOW_WIDTH / wAperture);
        hPixels = (WINDOW_HEIGHT / hAperture);

        focalPoint = CalculateFocalPoint();
    }

    Ray3 GetRayFromUV(float U, float V)
    {
        U -= 0.5f;
        V -= 0.5f;

        Float3 rayPos = worldPosition + right * U * wAperture + up * V * hAperture;
        Float3 rayDir = rayPos - focalPoint;

        return Ray3(rayPos, rayDir);
    }

    Float3 WorldToCamera(Float3 objectWorldPosition)
    {
        objectWorldPosition = objectWorldPosition - worldPosition;

        Float3 pixels = localTransformationMatrix * objectWorldPosition;
        pixels.z += focal_length;
        pixels.x *= wPixels;
        pixels.y *= hPixels;
        pixels.x /= abs(pixels.z / focal_length);
        pixels.y /= abs(pixels.z / focal_length);
        pixels.x += wHalfScreen;
        pixels.y += hHalfScreen;
        return pixels;
    }

protected:
    void RecalculateTransform() override
    {
        Transform::RecalculateTransform();
        focalPoint = CalculateFocalPoint();
    }
    
    Float3 CalculateFocalPoint() const
    {
        return worldPosition - forward * focal_length;
    }


    Float3 focalPoint;

    float aspect_ratio = 1.f;
    float wAperture = 1.f;
    float hAperture = 1.f;
    float wPixels;
    float hPixels;
    float wHalfScreen;
    float hHalfScreen;
};
