#include "stdafx.h"
#include "DrawableCube.h"

DrawableCube::DrawableCube(Float3 position, Transform* parent, float size): Transform(position, parent), size{size}
{
    const float halfSize = size / 2.0f;

    vertices[0] = std::make_unique<Transform>(Float3(-halfSize, -halfSize, -halfSize), this);
    vertices[1] = std::make_unique<Transform>(Float3(halfSize, -halfSize, -halfSize), this);
    vertices[2] = std::make_unique<Transform>(Float3(-halfSize, halfSize, -halfSize), this);
    vertices[3] = std::make_unique<Transform>(Float3(halfSize, halfSize, -halfSize), this);
    vertices[4] = std::make_unique<Transform>(Float3(-halfSize, -halfSize, halfSize), this);
    vertices[5] = std::make_unique<Transform>(Float3(halfSize, -halfSize, halfSize), this);
    vertices[6] = std::make_unique<Transform>(Float3(-halfSize, halfSize, halfSize), this);
    vertices[7] = std::make_unique<Transform>(Float3(halfSize, halfSize, halfSize), this);
}

void DrawableCube::Render(LineRenderer* lineRenderer, Camera* camera, Float3 color)
{
    const Float3 v1 = camera->WorldToCamera(vertices[0]->GetWorldPosition());
    const Float3 v2 = camera->WorldToCamera(vertices[1]->GetWorldPosition());
    const Float3 v3 = camera->WorldToCamera(vertices[2]->GetWorldPosition());
    const Float3 v4 = camera->WorldToCamera(vertices[3]->GetWorldPosition());
    const Float3 v5 = camera->WorldToCamera(vertices[4]->GetWorldPosition());
    const Float3 v6 = camera->WorldToCamera(vertices[5]->GetWorldPosition());
    const Float3 v7 = camera->WorldToCamera(vertices[6]->GetWorldPosition());
    const Float3 v8 = camera->WorldToCamera(vertices[7]->GetWorldPosition());

    lineRenderer->DrawLineFogApplied(&v1, &v2, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v2, &v4, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v4, &v3, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v3, &v1, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v5, &v6, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v6, &v8, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v8, &v7, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v7, &v5, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v1, &v5, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v2, &v6, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v4, &v8, Float3(1, 1, 1));
    lineRenderer->DrawLineFogApplied(&v3, &v7, Float3(1, 1, 1));
}
