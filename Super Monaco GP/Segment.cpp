#include "Segment.h"

#include "Camera.h"
#include "ModuleRenderer.h"

Segment::Segment(float zNear, unsigned int color) :
	zNear(zNear), zFar(zNear + SEGMENT_LENGTH), color(color)
{ }

Segment::~Segment()
{ }

float Segment::getZNear() const
{
	return zNear;
}

float Segment::getZFar() const
{
	return zFar;
}

void Segment::render(float offset, const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxScreenY) const
{
	if(camera->getIsBehind(offset + zFar)) return;

	Position3f worldPositionNearLeft{ ROAD_MIN_X, 0.0f, offset + zNear };
	Position3f worldPositionNearRight{ ROAD_MAX_X, 0.0f, offset + zNear };
	Position3f worldPositionFarLeft{ ROAD_MIN_X, 0.0f, offset + zFar };
	Position3f worldPositionFarRight{ ROAD_MAX_X, 0.0f, offset + zFar };

	Position2s screenPositionNearLeft, screenPositionNearRight;
	Position2s screenPositionFarLeft, screenPositionFarRight;

	camera->getPositionWorldToScreen(worldPositionNearLeft, screenPositionNearLeft);
	camera->getPositionWorldToScreen(worldPositionNearRight, screenPositionNearRight);
	camera->getPositionWorldToScreen(worldPositionFarLeft, screenPositionFarLeft);
	camera->getPositionWorldToScreen(worldPositionFarRight, screenPositionFarRight);

	if(screenPositionFarLeft.y >= maxScreenY) return;
	maxScreenY = screenPositionFarLeft.y;

	moduleRenderer->renderTrapezoid(screenPositionNearLeft, screenPositionNearRight, screenPositionFarRight, screenPositionFarLeft, color, true);
}