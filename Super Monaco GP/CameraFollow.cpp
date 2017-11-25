#include "CameraFollow.h"

#include "Globals.h"

CameraFollow::CameraFollow(const Road* road, const WorldPosition* followPosition) :
	Camera(road), followPosition(followPosition)
{ }

CameraFollow::~CameraFollow()
{ }

const WorldPosition* CameraFollow::getFollowPosition() const
{
	return followPosition;
}

void CameraFollow::update(float deltaTimeS)
{
	position.x = followPosition->x;
	position.y = followPosition->y + CAMERA_Y;
	position.z = followPosition->z - CAMERA_Y * getDepth();

	limitZ();
}