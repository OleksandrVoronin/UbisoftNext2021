#pragma once
#include <memory>

#include "IDrawable.h"

class DrawablePyramid : public IDrawable
{
public:
public:
    std::unique_ptr<Transform> vertices[5];

    DrawablePyramid(Float3 position, Transform* parent, float size);

    void Render(LineRenderer* lineRenderer, Camera* camera, Float3 color) override;

private:
    float size;
};
