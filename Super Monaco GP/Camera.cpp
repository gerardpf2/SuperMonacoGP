#include "Camera.h"

#include <math.h>

Camera::Camera()
{
	position.x = 0.0f;
	position.y = CAMERA_INITIAL_Y;
	position.z = -CAMERA_INITIAL_Y;

	depth = 1.0f / (float)tan((CAMERA_FOV / 2.0f) * PI / 180.0f);
}

Camera::~Camera()
{ }

const Position3f* Camera::getPosition() const
{
	return &position;
}

void Camera::follow(const Position3f& position)
{
	this->position.z = position.z - this->position.y;
}

void Camera::moveY(float incY)
{
	float positionY = position.y;

	position.y = fminf(fmaxf(position.y + incY, CAMERA_MIN_Y), CAMERA_MAX_Y);
	position.z -= position.y - positionY;
}

void Camera::moveXZ(float incX, float incZ)
{
	position.x = fminf(fmaxf(position.x + incX, -CAMERA_MAX_X), CAMERA_MAX_X);
	position.z += incZ;
}

void Camera::getPositionWorldToScreen(const Position3f& worldPosition, Position2i& screenPosition) const
{
	float scale = depth / (worldPosition.z - position.z);

	screenPosition.x = (int)roundf((WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 2.0f) * scale * (worldPosition.x - position.x));
	screenPosition.y = (int)roundf((WINDOW_HEIGHT / 2.0f) - (WINDOW_HEIGHT / 2.0f) * scale * (worldPosition.y - position.y));
}