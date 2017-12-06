#include "CameraFollow.h"

#include "Globals.h"

CameraFollow::CameraFollow(bool forward, const Road* road, const WorldPosition* followPosition, const WorldPosition& offsetPosition) :
	Camera(forward, road), followPosition(followPosition), offsetPosition(offsetPosition)
{ }

CameraFollow::~CameraFollow()
{ }

const WorldPosition* CameraFollow::getFollowPosition() const
{
	return followPosition;
}

void CameraFollow::update(float deltaTimeS)
{
	position.x = followPosition->x + offsetPosition.x;
	position.y = followPosition->y + CAMERA_Y + offsetPosition.y;
	position.z = followPosition->z + getOffsetZ() + offsetPosition.z;

	limitZ();
}