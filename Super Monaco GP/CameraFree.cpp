#include "CameraFree.h"

#include "Road.h"
#include "Utils.h"
#include "Segment.h"
#include "ModuleInput.h"

CameraFree::CameraFree(const Road* road, const ModuleInput* moduleInput) :
	Camera(road), moduleInput(moduleInput)
{ }

CameraFree::~CameraFree()
{ }

void CameraFree::update(float deltaTimeS)
{
	Segment* segment = road->getSegmentAtZ(position.z);
	position.y = CAMERA_Y + interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar());

	float speedMultiplier = moduleInput->getKeyPressed(SDL_SCANCODE_SPACE) ? 5.0f : 1.0f;

	if(moduleInput->getKeyPressed(SDL_SCANCODE_UP)) moveZ(speedMultiplier * CAMERA_FREE_SPEED_Z * deltaTimeS);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_DOWN)) moveZ(speedMultiplier * -CAMERA_FREE_SPEED_Z * deltaTimeS);
}

void CameraFree::moveZ(float incZ)
{
	position.z += incZ;

	limitZ();
}