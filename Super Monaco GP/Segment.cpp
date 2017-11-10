#include "Segment.h"

#include "Camera.h"
#include "ModuleRenderer.h"

Segment::Segment(unsigned int index, float zNear, unsigned int color) :
	index(index), zNear(zNear), zFar(zNear + SEGMENT_LENGTH), color(color)
{ }

Segment::~Segment()
{ }

unsigned int Segment::getIndex() const
{
	return index;
}

float Segment::getZNear() const
{
	return zNear;
}

float Segment::getZFar() const
{
	return zFar;
}

void Segment::render(float xOffset, float dXOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxScreenY) const
{
	if(camera->getIsBehind(zOffset + zFar)) return;

	Position3f worldPositionNearLeft{ ROAD_MIN_X - xOffset, 0.0f, zOffset + zNear };
	Position3f worldPositionNearRight{ ROAD_MAX_X - xOffset, 0.0f, zOffset + zNear };
	Position3f worldPositionFarLeft{ ROAD_MIN_X - xOffset - dXOffset, 0.0f, zOffset + zFar };
	Position3f worldPositionFarRight{ ROAD_MAX_X - xOffset - dXOffset, 0.0f, zOffset + zFar };

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