#include "Camera.h"

#include "Road.h"
#include "Utils.h"

Camera::Camera(bool forward, const Road* road, float projectionY0, float projectionY1) :
	forward(forward), road(road), projectionY0(projectionY0), projectionY1(projectionY1)
{
	// This should happen: 1 / projectionY0 + 1 / projectionY1 = 1

	depth = 1.0f / tanf(degToRad(CAMERA_FOV / 2.0f));

	position.x = 0.0f;
	position.y = CAMERA_Y;
	position.z = getOffsetZ();

	limitZ();
}

Camera::~Camera()
{ }

bool Camera::getForward() const
{
	return forward;
}

const Road* Camera::getRoad() const
{
	return road;
}

float Camera::getProjectionY0() const
{
	return projectionY0;
}

float Camera::getProjectionY1() const
{
	return projectionY1;
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
	return CAMERA_Y * depth * (forward ? -1.0f : 1.0f);
}

float Camera::getBasePositionZ() const
{
	return position.z - getOffsetZ();
}

bool Camera::isBehind(float z) const
{
	return (forward ? 1.0f : -1.0f) * (z - position.z) <= depth;
}

void Camera::project(const WorldPosition& worldPosition, WindowPosition& windowPosition) const
{
	float scale = depth / ((forward ? 1.0f : -1.0f) * (worldPosition.z - position.z));

	windowPosition.x = (short)roundf((WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 2.0f) * scale * (worldPosition.x - position.x));
	windowPosition.y = (short)roundf((WINDOW_HEIGHT / projectionY0) - (WINDOW_HEIGHT / projectionY1) * scale * (worldPosition.y - position.y));
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
	position.z = mod0L(position.z, road->getLength());
}