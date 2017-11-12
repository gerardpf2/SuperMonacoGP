#include "Camera.h"

#include "Road.h"
#include "Segment.h"

Camera::Camera(const Road* road) :
	road(road)
{
	depth = 1.0f / (float)tan(degToRad(CAMERA_FOV / 2.0f));

	position.x = 0.0f;
	position.y = CAMERA_Y;
	position.z = -position.y * depth;

	updateLimitZRoad();
}

Camera::~Camera()
{ }

const Position3f* Camera::getPosition() const
{
	return &position;
}

void Camera::update(float deltaTimeS)
{
	Segment* segment = road->getSegmentAtZ(position.z);

	float zNear = segment->getZNear();
	float zFar = segment->getZFar();

	float yNear = segment->yNear;
	float yFar = segment->yFar;

	float y = yNear + ((position.z - zNear) / (zFar - zNear)) * (yFar - yNear);

	position.y = CAMERA_Y + y;

	updateLimitZRoad();
}

/* float Camera::getBaseZ() const
{
	return position.z + position.y * depth;
} */

bool Camera::getIsBehind(float z) const
{
	return z - position.z <= depth;
}

void Camera::getPositionWorldToScreen(const Position3f& worldPosition, Position2s& screenPosition) const
{
	float scale = depth / (worldPosition.z - position.z);

	screenPosition.x = (short)roundf((WINDOW_WIDTH / 2.0f) + (WINDOW_WIDTH / 2.0f) * scale * (worldPosition.x - position.x));
	screenPosition.y = (short)roundf((WINDOW_HEIGHT / 2.0f) - (WINDOW_HEIGHT / 2.0f) * scale * (worldPosition.y - position.y));
}

void Camera::updateLimitZRoad()
{
	position.z = modF0ToL(position.z, road->getLength());
}