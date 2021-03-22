#include "stdafx.h"
#include "LineRenderer.h"
#include "../App/main.h"

#include <algorithm>

void LineRenderer::RenderFrame()
{
    // Fill the background if fog color isn't black
    if (fogColor.LengthSquared())
    {
        for (int i = 0; i < WINDOW_HEIGHT; i++)
        {
            App::DrawLine(0, static_cast<float>(i), static_cast<float>(WINDOW_WIDTH), static_cast<float>(i), fogColor.x,
                          fogColor.y,
                          fogColor.z);
        }
    }

    std::sort(renderInstructionsQueue.begin(), renderInstructionsQueue.end());
    for (int i = 0; i < renderInstructionsQueue.size(); i++)
    {
        RenderLineFinal(renderInstructionsQueue[i].startPosition, renderInstructionsQueue[i].endPosition,
                        renderInstructionsQueue[i].color);
    }

    renderInstructionsQueue.clear();
}

void LineRenderer::DrawLine(const Float3* a, const Float3* b, Float3 color)
{
    renderInstructionsQueue.push_back(LineRenderInstruction{
        *a, *b,
        color
    });
}

void LineRenderer::DrawGradientLineFogApplied(const Float3* a, const Float3* b, Float3 colorA, Float3 colorB,
                                              float worldLength)
{
    if (IsPositionCulled(a, b)) return;

    int segments = 1;
    if (worldLength > 0)
    {
        segments = static_cast<int>(min(maxSegments, ceil(worldLength / lineSegmentationDistance)));
    }
    const float segmentLength = 1.0f / static_cast<float>(segments);

    for (int i = 0; i < segments; i++)
    {
        Float3 segmentStart = Float3::Lerp(a, b, i * segmentLength);
        Float3 segmentEnd = Float3::Lerp(a, b, (i + 1) * segmentLength);
        const Float3 color = Float3::Lerp(colorA, colorB, (i + 1) * segmentLength);

        if (IsPositionCulled(&segmentStart, &segmentEnd))
        {
            continue;
        }

        renderInstructionsQueue.push_back(LineRenderInstruction{
            segmentStart, segmentEnd,
            Float3::Lerp(color, fogColor, GetFogAmountAtZ(segmentStart.z))
        });
    }
}

void LineRenderer::DrawLineFogApplied(const Float3* a, const Float3* b, Float3 color)
{
    if (IsPositionCulled(a, b)) return;

    const float totalZSpread = abs(a->z - b->z);
    int segments = 1;
    if (totalZSpread > 0)
    {
        segments = static_cast<int>(min(maxSegments, ceil(totalZSpread / lineSegmentationDistance)));
    }
    const float segmentLength = 1.0f / static_cast<float>(segments);

    for (int i = 0; i < segments; i++)
    {
        Float3 segmentStart = Float3::Lerp(a, b, i * segmentLength);
        Float3 segmentEnd = Float3::Lerp(a, b, (i + 1) * segmentLength);

        if (IsPositionCulled(&segmentStart, &segmentEnd))
        {
            continue;
        }

        renderInstructionsQueue.push_back(LineRenderInstruction{
            segmentStart, segmentEnd,
            Float3::Lerp(color, fogColor, GetFogAmountAtZ(segmentStart.z))
        });
    }
}

float LineRenderer::GetFogAmountAtZ(float z) const
{
    if (z < fogStartZ) return 0;
    if (z > fogFullZ) return 1;

    return (z - fogStartZ) / (fogFullZ - fogStartZ);
}

bool LineRenderer::IsPositionCulled(const Float3* a, const Float3* b) const
{
    if ((a->x < 0 || a->x > WINDOW_WIDTH
            || a->y < 0 || a->y > WINDOW_HEIGHT)
        && (b->x < 0 || b->x > WINDOW_WIDTH
            || b->y < 0 || b->y > WINDOW_HEIGHT)
        || (a->z > fogFullZ && b->z > fogFullZ)
        || (a->z < 0 && b->z < 0))
    {
        return true;
    }

    return false;
}
