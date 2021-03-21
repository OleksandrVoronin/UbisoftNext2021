#pragma once
#include "../Math/Float3.h"
#include "../Utils/Camera.h"
#include "../Utils/LineRenderer.h"
#include "../Utils/Transform.h"

class IDrawable : virtual public Transform
{
public:

    virtual void Render(LineRenderer* lineRenderer, Camera* camera, Float3 color) = 0;
};
