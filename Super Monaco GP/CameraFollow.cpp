#include "CameraFollow.h"

CameraFollow::CameraFollow(const float* followZ, const Road* road) :
	Camera(road), followZ(followZ)
{ }

CameraFollow::~CameraFollow()
{ }

void CameraFollow::update(float deltaTimeS)
{
	position.z = *followZ - position.y * depth;

	updateLimitZRoad();
}