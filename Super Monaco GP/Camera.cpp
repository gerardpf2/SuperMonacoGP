#include "Camera.h"

#include "Road.h"

Camera::Camera(const Road* road) :
	road(road)
{
	depth = 1.0f / (float)tan((CAMERA_FOV / 2.0f) * PI / 180.0f);

	position.x = 0.0f;
	position.y = CAMERA_INITIAL_Y;
	position.z = -CAMERA_INITIAL_Y * depth;
}

Camera::~Camera()
{ }

void Camera::update(float deltaTimeS)
{ }

float Camera::getBaseZ() const
{
	return position.z + position.y * depth;
}

bool Camera::getIsBehind(float z) const
{
	return z <= getBaseZ();
}

void Camera::getPositionWorldToScreen(const Position3f& worldPosition, Position2s& screenPosition) const
{
	float scale = depth / (worldPosition.z - position.z);

	screenPosition.x = (short)roundf((WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 2.0f) * scale * (worldPosition.x - position.x));
	screenPosition.y = (short)roundf((WINDOW_HEIGHT / 2.0f) - (WINDOW_HEIGHT / 2.0f) * scale * (worldPosition.y - position.y));
}

void Camera::updateLimitZRoad()
{
	position.z = modF0ToL(getBaseZ(), road->getLength()) - position.y * depth;
}