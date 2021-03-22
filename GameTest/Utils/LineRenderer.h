#pragma once
#include <algorithm>
#include <set>

#include "../App/app.h"
#include "../Math/Float3.h"

// Slightly Better OpenGL Free Renderer
class LineRenderer
{
public:
    // Some constants
    const float lineSegmentationDistance = 1.0f;
    const int maxSegments = 10;
    const float fogStartZ = 51;
    const float fogFullZ = 51.4f;
    const Float3 fogColor{0, 0, 0};

    LineRenderer()
    {
    }

    LineRenderer(Float3 fogColor) : fogColor{fogColor}
    {
    }

    LineRenderer(float fogStartZ, float fogFullZ) : fogStartZ{fogStartZ},
                                                    fogFullZ{fogFullZ}, fogColor{fogColor}
    {
    }

    ~LineRenderer()
    {
        renderInstructionsQueue.clear();
    }

    void RenderFrame();

    void DrawLine(const Float3* a, const Float3* b, Float3 color);

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

        bool LineRenderInstruction::operator <(const LineRenderInstruction& b) const
        {
            return this->midPointZ > b.midPointZ;
        }


    private:
        float midPointZ;
    };

    std::vector<LineRenderInstruction> renderInstructionsQueue;

    float fogAmount(float z) const;

    bool isPositionCulled(const Float3* a, const Float3* b) const;

    /*
    * a & b are pixel-coordinates where z is world-space distance
    */
    void LineRenderer::RenderLineFinal(Float3 a, Float3 b, Float3 color)
    {
        if (a.z < 0 && b.z < 0) return;

        App::DrawLine(a.x, a.y, b.x, b.y, color.x, color.y, color.z);
    }
};
