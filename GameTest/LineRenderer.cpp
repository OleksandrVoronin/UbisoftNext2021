#include "stdafx.h"
#include "LineRenderer.h"

#include <algorithm>

void LineRenderer::RenderFrame()
{
	// Fill the background if fog color isn't black
	if (fogColor.LengthSquared())
	{
		for (int i = 0; i < WINDOW_HEIGHT; i++)
		{
			App::DrawLine(0, i, WINDOW_WIDTH, i, fogColor.x, fogColor.y, fogColor.z);
		}
	}

	std::sort(renderInstructionsQueue.begin(), renderInstructionsQueue.end());
	for (int i = 0; i < renderInstructionsQueue.size(); i++) {
		DrawLineSingleColor(renderInstructionsQueue[i].startPosition, renderInstructionsQueue[i].endPosition, renderInstructionsQueue[i].color);
	}

	renderInstructionsQueue.clear();
}

void LineRenderer::DrawLineSingleColor(Float3 a, Float3 b, Float3 color)
{
	if (a.z < 0 && b.z < 0) return;

	App::DrawLine(a.x, a.y, b.x, b.y, color.x, color.y, color.z);
}

void LineRenderer::DrawGradientLineFogApplied(const Float3* a, const Float3* b, Float3 colorA, Float3 colorB,
	float worldLength)
{
	if (isPositionCulled(a, b)) return;

	int segments = 1;
	if (worldLength > 0)
	{
		segments = static_cast<int>(min(maxSegments, ceil(worldLength / lineSegmentationDistance)));
	}
	float segmentLength = 1.0f / (float)segments;

	for (int i = 0; i < segments; i++)
	{
		Float3 segmentStart = Float3::Lerp(a, b, i * segmentLength);
		Float3 segmentEnd = Float3::Lerp(a, b, (i + 1) * segmentLength);
		Float3 color = Float3::Lerp(colorA, colorB, (i + 1) * segmentLength);

		if (isPositionCulled(&segmentStart, &segmentEnd))
		{
			continue;
		}

		renderInstructionsQueue.push_back(LineRenderInstruction{
			segmentStart, segmentEnd,
			Float3::Lerp(color, fogColor, fogAmount(segmentStart.z))
			});
	}
}

void LineRenderer::DrawLineFogApplied(const Float3* a, const Float3* b, Float3 color)
{
	if (isPositionCulled(a, b)) return;

	float totalZSpread = abs(a->z - b->z);
	int segments = 1;
	if (totalZSpread > 0)
	{
		segments = static_cast<int>(min(maxSegments, ceil(totalZSpread / lineSegmentationDistance)));
	}
	float segmentLength = 1.0f / (float)segments;

	for (int i = 0; i < segments; i++)
	{
		Float3 segmentStart = Float3::Lerp(a, b, i * segmentLength);
		Float3 segmentEnd = Float3::Lerp(a, b, (i + 1) * segmentLength);

		if (isPositionCulled(&segmentStart, &segmentEnd))
		{
			continue;
		}

		renderInstructionsQueue.push_back(LineRenderInstruction{
			segmentStart, segmentEnd,
			Float3::Lerp(color, fogColor, fogAmount(segmentStart.z))
			});
	}
}

void LineRenderer::DrawDottedLineFogApplied(const Float3* a, const Float3* b, Float3 color, float worldLength)
{
	if (isPositionCulled(a, b)) return;

	int segments = 1;
	if (worldLength > 0)
	{
		segments = static_cast<int>(min(maxSegments * 8, ceil(worldLength / lineSegmentationDistance)));
	}
	if (segments % 2 == 0)
	{
		segments++;
	}
	float segmentLength = 1.0f / (float)segments;

	for (int i = 0; i < segments; i += 2)
	{
		Float3 segmentStart = Float3::Lerp(a, b, i * segmentLength);
		Float3 segmentEnd = Float3::Lerp(a, b, (i + 1) * segmentLength);

		if (isPositionCulled(&segmentStart, &segmentEnd))
		{
			continue;
		}

		renderInstructionsQueue.push_back(LineRenderInstruction{
			segmentStart, segmentEnd,
			Float3::Lerp(color, fogColor, fogAmount(segmentStart.z))
			});
	}
}

float LineRenderer::fogAmount(float z) const
{
	if (z < fogStartZ) return 0;
	if (z > fogFullZ) return 1;

	return (z - fogStartZ) / (fogFullZ - fogStartZ);
}

bool LineRenderer::isPositionCulled(const Float3* a, const Float3* b) const
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
