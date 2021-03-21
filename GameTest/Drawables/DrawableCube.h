#pragma once
#include <memory>

#include "IDrawable.h"
#include "../Utils/LineRenderer.h"
#include "../App/app.h"

class DrawableCube : public IDrawable
{
public:
    std::unique_ptr<Transform> vertices[8];

    DrawableCube(Float3 position, Transform* parent, float size);

    void Render(LineRenderer* lineRenderer, Camera* camera, Float3 color) override;

private:
    float size;
};
