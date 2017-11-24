#include "CameraFollow.h"

#include "Globals.h"

CameraFollow::CameraFollow(const WorldPosition* followPosition, const Road* road) :
	Camera(road), followPosition(followPosition)
{ }

CameraFollow::~CameraFollow()
{ }

void CameraFollow::update(float deltaTimeS)
{
	position.x = followPosition->x;
	position.y = followPosition->y + CAMERA_Y;
	position.z = followPosition->z - CAMERA_Y * depth;

	limitZ();
}