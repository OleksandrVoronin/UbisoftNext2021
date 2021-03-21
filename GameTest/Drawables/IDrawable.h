#pragma once
#include "../Camera.h"
#include "../LineRenderer.h"
#include "../Transform.h"

class IDrawable : virtual public Transform
{
public:

    virtual void Render(LineRenderer* lineRenderer, Camera* camera, Float3 color) = 0;
};
