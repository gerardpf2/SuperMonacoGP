#include "Camera.h"

#include "Road.h"
#include "Utils.h"
#include "Segment.h"

Camera::Camera(const Road* road) :
	road(road)
{
	depth = 1.0f / (float)tan(degToRad(CAMERA_FOV / 2.0f));

	position.first = 0.0f;
	position.second = CAMERA_Y;
	position.third = -position.second * depth;

	updateLimitZRoad();
}

Camera::~Camera()
{ }

const WorldPosition* Camera::getPosition() const
{
	return &position;
}

void Camera::update(float deltaTimeS)
{
	Segment* segment = road->getSegmentAtZ(position.third);

	position.second = CAMERA_Y + linear(position.third, segment->getZNear(), segment->getZFar(), segment->yNear, segment->yFar);

	updateLimitZRoad();
}

/* float Camera::getBaseZ() const
{
	return position.third + position.second * depth;
} */

bool Camera::getIsBehind(float z) const
{
	return z - position.third <= depth;
}

void Camera::project(const WorldPosition& worldPosition, ScreenPosition& screenPosition) const
{
	float scale = depth / (worldPosition.third - position.third);

	screenPosition.first = (short)roundf((WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 2.0f) * scale * (worldPosition.first - position.first));
	screenPosition.second = (short)roundf((WINDOW_HEIGHT / 2.0f) - (WINDOW_HEIGHT / 2.0f) * scale * (worldPosition.second - position.second));
}

void Camera::project(const WorldPosition& worldPositionBL, const WorldPosition& worldPositionBR, const WorldPosition& worldPositionUR, const WorldPosition& worldPositionUL, ScreenPosition& screenPositionBL, ScreenPosition& screenPositionBR, ScreenPosition& screenPositionUR, ScreenPosition& screenPositionUL) const
{
	project(worldPositionBL, screenPositionBL);
	project(worldPositionBR, screenPositionBR);
	project(worldPositionUR, screenPositionUR);
	project(worldPositionUL, screenPositionUL);
}

void Camera::updateLimitZRoad()
{
	position.third = modF0ToL(position.third, road->getLength());
}