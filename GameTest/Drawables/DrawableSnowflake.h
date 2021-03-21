#pragma once
#include "IDrawable.h"


class DrawableSnowflake : public IDrawable
{
public:
    std::unique_ptr<Transform> vertices[21];

    DrawableSnowflake(Float3 position, Transform* parent, float size) : Transform(position, parent), size{size}
    {
        const float halfSize = size / 2.0f;

        vertices[0] = std::make_unique<Transform>(Float3(0, 0, 0), this);

        vertices[1] = std::make_unique<Transform>(Float3(-halfSize, 0, 0), this);
        vertices[2] = std::make_unique<Transform>(Float3(0, halfSize, 0), this);
        vertices[3] = std::make_unique<Transform>(Float3(halfSize, 0, 0), this);
        vertices[4] = std::make_unique<Transform>(Float3(0, -halfSize, 0), this);

        vertices[5] = std::make_unique<Transform>(Float3(-halfSize * 0.71f, halfSize * 0.71f, 0), this);
        vertices[6] = std::make_unique<Transform>(Float3(halfSize * 0.71f, halfSize * 0.71f, 0), this);
        vertices[7] = std::make_unique<Transform>(Float3(halfSize * 0.71f, -halfSize * 0.71f, 0), this);
        vertices[8] = std::make_unique<Transform>(Float3(-halfSize * 0.71f, -halfSize * 0.71f, 0), this);

        vertices[9] = std::make_unique<Transform>(Float3(-halfSize * 0.5f, halfSize * 0.5f, 0), this);
        vertices[10] = std::make_unique<Transform>(Float3(-halfSize * 0.5f, halfSize * 0.71f, 0), this);
        vertices[11] = std::make_unique<Transform>(Float3(-halfSize * 0.71f, halfSize * 0.5f, 0), this);

        vertices[12] = std::make_unique<Transform>(Float3(halfSize * 0.5f, halfSize * 0.5f, 0), this);
        vertices[13] = std::make_unique<Transform>(Float3(halfSize * 0.5f, halfSize * 0.71f, 0), this);
        vertices[14] = std::make_unique<Transform>(Float3(halfSize * 0.71f, halfSize * 0.5f, 0), this);

        vertices[15] = std::make_unique<Transform>(Float3(halfSize * 0.5f, -halfSize * 0.5f, 0), this);
        vertices[16] = std::make_unique<Transform>(Float3(halfSize * 0.5f, -halfSize * 0.71f, 0), this);
        vertices[17] = std::make_unique<Transform>(Float3(halfSize * 0.71f, -halfSize * 0.5f, 0), this);

        vertices[18] = std::make_unique<Transform>(Float3(-halfSize * 0.5f, -halfSize * 0.5f, 0), this);
        vertices[19] = std::make_unique<Transform>(Float3(-halfSize * 0.5f, -halfSize * 0.71f, 0), this);
        vertices[20] = std::make_unique<Transform>(Float3(-halfSize * 0.71f, -halfSize * 0.5f, 0), this);
    }

    void Render(LineRenderer* lineRenderer, Camera* camera, Float3 color) override
    {
        Float3 worldPoints[21];
        for (int i = 0; i < 21; i++)
        {
            worldPoints[i] = camera->WorldToCamera(vertices[i]->GetWorldPosition());
        }

        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[1], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[2], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[3], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[4], color);

        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[5], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[6], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[7], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[0], &worldPoints[8], color);

        lineRenderer->DrawLineFogApplied(&worldPoints[10], &worldPoints[9], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[11], &worldPoints[9], color);

        lineRenderer->DrawLineFogApplied(&worldPoints[13], &worldPoints[12], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[14], &worldPoints[12], color);

        lineRenderer->DrawLineFogApplied(&worldPoints[16], &worldPoints[15], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[17], &worldPoints[15], color);

        lineRenderer->DrawLineFogApplied(&worldPoints[19], &worldPoints[18], color);
        lineRenderer->DrawLineFogApplied(&worldPoints[20], &worldPoints[18], color);
    }

private:
    float size;
};
