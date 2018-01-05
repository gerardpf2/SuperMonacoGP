#include "CameraFollow.h"

#include <assert.h>
#include "Globals.h"

CameraFollow::CameraFollow(bool forward, const Road* road, const WorldPosition* followPosition, float projectionY0, float projectionY1, const WorldPosition& offsetPosition) :
	Camera(forward, road, projectionY0, projectionY1), followPosition(followPosition), offsetPosition(offsetPosition)
{ }

CameraFollow::~CameraFollow()
{ }

const WorldPosition* CameraFollow::getFollowPosition() const
{
	return followPosition;
}

void CameraFollow::update(float deltaTimeS)
{
	assert(followPosition);

	position.x = followPosition->x + offsetPosition.x;
	position.y = followPosition->y + CAMERA_Y + offsetPosition.y;
	position.z = followPosition->z + getOffsetZ() + offsetPosition.z;

	limitZ();
}