#include "Camera.h"

#include <math.h>

Camera::Camera(const Position3f& position, float fieldOfView) :
	position(position), fieldOfView(fieldOfView)
{
	depth = 1.0f / (float)tan((fieldOfView / 2.0f) * PI / 180.0f);
}

Camera::~Camera()
{ }

void Camera::getPositionWorldToScreen(const Position3f& worldPosition, Position2i& screenPosition) const
{
	float scale = depth / (worldPosition.z - position.z);

	screenPosition.x = (int)roundf((WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 2.0f) * scale * (worldPosition.x - position.x));
	screenPosition.y = (int)roundf((WINDOW_HEIGHT / 2.0f) - (WINDOW_HEIGHT / 2.0f) * scale * (worldPosition.y - position.y));
}