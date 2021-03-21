#pragma once
#include "IDrawable.h"

class DrawableGem : public IDrawable
{
public:
    std::unique_ptr<Transform> vertices[11];

    DrawableGem(Float3 position, Transform* parent, float size) : Transform(position, parent), size{size}
    {
        const float halfSize = size / 2.0f;

        vertices[0] = std::make_unique<Transform>(Float3(0, 0, 0), this);

        vertices[1] = std::make_unique<Transform>(Float3(0.31f * halfSize, halfSize, 0.95f * halfSize), this);
        vertices[2] = std::make_unique<Transform>(Float3(halfSize, halfSize, 0), this);
        vertices[3] = std::make_unique<Transform>(Float3(0.31f * halfSize, halfSize, -0.95f * halfSize), this);
        vertices[4] = std::make_unique<Transform>(Float3(-0.81f * halfSize, halfSize, -0.59f * halfSize), this);
        vertices[5] = std::make_unique<Transform>(Float3(-0.81f * halfSize, halfSize, 0.59f * halfSize), this);

        const float upperHalfSize = size / 2.0f * 0.4f;
        vertices[6] = std::make_unique<Transform>(
            Float3(0.31f * upperHalfSize, halfSize + upperHalfSize, 0.95f * upperHalfSize), this);
        vertices[7] = std::make_unique<Transform>(Float3(upperHalfSize, halfSize + upperHalfSize, 0), this);
        vertices[8] = std::make_unique<Transform>(
            Float3(0.31f * upperHalfSize, halfSize + upperHalfSize, -0.95f * upperHalfSize), this);
        vertices[9] = std::make_unique<Transform>(
            Float3(-0.81f * upperHalfSize, halfSize + upperHalfSize, -0.59f * upperHalfSize), this);
        vertices[10] = std::make_unique<Transform>(
            Float3(-0.81f * upperHalfSize, halfSize + upperHalfSize, 0.59f * upperHalfSize), this);
    }

    void Render(LineRenderer* lineRenderer, Camera* camera, Float3 color) override
    {
        Float3 worldPoints[11];
        for (int i = 0; i < 11; i++)
        {
            worldPoints[i] = camera->WorldToCamera(vertices[i]->GetWorldPosition());
        }

        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[1], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[2], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[3], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[4], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[5], color);

        lineRenderer->DrawLineFogApplied(&worldPoints[2], &worldPoints[1], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[3], &worldPoints[2], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[4], &worldPoints[3], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[5], &worldPoints[4], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[1], &worldPoints[5], color);

        lineRenderer->DrawLineFogApplied(&worldPoints[6], &worldPoints[1], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[7], &worldPoints[2], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[8], &worldPoints[3], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[9], &worldPoints[4], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[10], &worldPoints[5], color);

        lineRenderer->DrawLineFogApplied(&worldPoints[6], &worldPoints[7], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[7], &worldPoints[8], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[8], &worldPoints[9], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[9], &worldPoints[10], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[10], &worldPoints[6], color);
    }

private:
    float size;
};
