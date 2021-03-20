#pragma once
#include <algorithm>
#include <set>

#include "App/app.h"
#include "App/main.h"
#include "Math/Float3.h"

// Some constants
const float lineSegmentationDistance = 1.0f;
const int maxSegments = 10;
const float fogStartZ = 30;
const float fogFullZ = 55.0f;
const Float3 fogColor {0,0,0};

// Slightly Better OpenGL Free Renderer
class LineRenderer
{
public:

    void RenderFrame();

    /*
     * a & b are pixel-coordinates where z is world-space distance
     */
    static void DrawLineSingleColor(Float3 a, Float3 b, Float3 color);
    
    void DrawGradientLineFogApplied(const Float3* a, const Float3* b, Float3 colorA, Float3 colorB, float worldLength);

    void DrawLineFogApplied(const Float3* a, const Float3* b, Float3 color);

    void DrawDottedLineFogApplied(const Float3* a, const Float3* b, Float3 color, float worldLength);

private:
    class LineRenderInstruction
    {
    public:
        Float3 startPosition;
        Float3 endPosition;

        Float3 color;

        LineRenderInstruction(Float3 startPosition, Float3 endPosition,
            Float3 color) : startPosition{startPosition}, endPosition{endPosition},
                color{color}
        {
            midPointZ = (startPosition + endPosition).z / 2.0f;
        }

        bool LineRenderInstruction::operator <(const LineRenderInstruction &b) const
        {
            return this->midPointZ > b.midPointZ;
        }


    private:
        float midPointZ;
    };

    std::vector<LineRenderInstruction> renderInstructionsQueue;

    static float fogAmount(float z);

    static bool isPositionCulled(const Float3* a, const Float3* b);
};
