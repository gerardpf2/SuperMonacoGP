#include "CameraFollow.h"

CameraFollow::CameraFollow(const Position3f* followPosition, const Road* road) :
	Camera(road), followPosition(followPosition)
{ }

CameraFollow::~CameraFollow()
{ }

void CameraFollow::update(float deltaTimeS)
{
	Camera::update(deltaTimeS);

	position.first = followPosition->first;
	position.third = followPosition->third - position.second * depth;

	updateLimitZRoad();
}