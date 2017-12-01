#include "Camera.h"

#include "Road.h"
#include "Utils.h"

Camera::Camera(const Road* road) :
	road(road)
{
	depth = 1.0f / tanf(degToRad(CAMERA_FOV / 2.0f));

	position.x = 0.0f;
	position.y = CAMERA_Y;
	position.z = -getOffsetZ();

	limitZ();
}

Camera::~Camera()
{ }

const Road* Camera::getRoad() const
{
	return road;
}

float Camera::getDepth() const
{
	return depth;
}

const WorldPosition* Camera::getPosition() const
{
	return &position;
}

float Camera::getOffsetZ() const
{
	return CAMERA_Y * depth;
}

float Camera::getBasePositionZ() const
{
	return position.z + getOffsetZ();
}

bool Camera::isBehind(float z) const
{
	return z + depth <= getBasePositionZ();
}

void Camera::project(const WorldPosition& worldPosition, WindowPosition& windowPosition) const
{
	float scale = depth / (worldPosition.z - position.z);

	windowPosition.x = (short)roundf((WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 2.0f) * scale * (worldPosition.x - position.x));
	windowPosition.y = (short)roundf((WINDOW_HEIGHT / 1.5f) - (WINDOW_HEIGHT / 3.0f) * scale * (worldPosition.y - position.y));
}

void Camera::project(const WorldTrapezoid& worldTrapezoid, WindowTrapezoid& windowTrapezoid) const
{
	project(worldTrapezoid.nl, windowTrapezoid.nl);
	project(worldTrapezoid.nr, windowTrapezoid.nr);
	project(worldTrapezoid.fr, windowTrapezoid.fr);
	project(worldTrapezoid.fl, windowTrapezoid.fl);
}

void Camera::update(float deltaTimeS)
{ }

void Camera::limitZ()
{
	position.z = modF0ToL(getBasePositionZ(), road->getLength()) - getOffsetZ();
}