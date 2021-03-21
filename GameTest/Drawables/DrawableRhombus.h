#pragma once
#include "IDrawable.h"


class DrawableRhombus : public IDrawable
{
public:
    std::unique_ptr<Transform> vertices[6];

    DrawableRhombus(Float3 position, Transform* parent, float size) : Transform(position, parent), size{size}
    {
        const float halfSize = size / 2.0f;

        vertices[0] = std::make_unique<Transform>(Float3(halfSize, 0, halfSize), this);
        vertices[1] = std::make_unique<Transform>(Float3(-halfSize, 0, halfSize), this);
        vertices[2] = std::make_unique<Transform>(Float3(-halfSize, 0, -halfSize), this);
        vertices[3] = std::make_unique<Transform>(Float3(halfSize, 0, -halfSize), this);
        vertices[4] = std::make_unique<Transform>(Float3(0, halfSize, 0), this);
        vertices[5] = std::make_unique<Transform>(Float3(0, -halfSize, 0), this);
    }

    void Render(LineRenderer* lineRenderer, Camera* camera, Float3 color) override
    {
        const Float3 v1 = camera->WorldToCamera(vertices[0]->GetWorldPosition());
        const Float3 v2 = camera->WorldToCamera(vertices[1]->GetWorldPosition());
        const Float3 v3 = camera->WorldToCamera(vertices[2]->GetWorldPosition());
        const Float3 v4 = camera->WorldToCamera(vertices[3]->GetWorldPosition());
        const Float3 v5 = camera->WorldToCamera(vertices[4]->GetWorldPosition());
        const Float3 v6 = camera->WorldToCamera(vertices[5]->GetWorldPosition());

        lineRenderer->DrawLineFogApplied(&v1, &v2, color);
        lineRenderer->DrawLineFogApplied(&v2, &v3, color);
        lineRenderer->DrawLineFogApplied(&v3, &v4, color);
        lineRenderer->DrawLineFogApplied(&v4, &v1, color);
        lineRenderer->DrawLineFogApplied(&v5, &v1, color);
        lineRenderer->DrawLineFogApplied(&v5, &v2, color);
        lineRenderer->DrawLineFogApplied(&v5, &v3, color);
        lineRenderer->DrawLineFogApplied(&v5, &v4, color);
        lineRenderer->DrawLineFogApplied(&v6, &v1, color);
        lineRenderer->DrawLineFogApplied(&v6, &v2, color);
        lineRenderer->DrawLineFogApplied(&v6, &v3, color);
        lineRenderer->DrawLineFogApplied(&v6, &v4, color);
    }

private:
    float size;
};
