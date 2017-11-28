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

const ModuleInput* CameraFree::getModuleInput() const
{
	return moduleInput;
}

void CameraFree::update(float deltaTimeS)
{
	Segment* segment = getRoad()->getSegmentAtZ(position.z);

	float a = interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar());

	// position.y = CAMERA_Y + interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar());

	float speedMultiplier = moduleInput->isKeyPressed(SDL_SCANCODE_SPACE) ? 5.0f : 1.0f;

	if(moduleInput->isKeyPressed(SDL_SCANCODE_UP)) moveZ(speedMultiplier * CAMERA_FREE_SPEED_Z * deltaTimeS);
	if(moduleInput->isKeyPressed(SDL_SCANCODE_DOWN)) moveZ(speedMultiplier * -CAMERA_FREE_SPEED_Z * deltaTimeS);
}

void CameraFree::moveZ(float incZ)
{
	

	Segment* segment = getRoad()->getSegmentAtZ(position.z);
	position.y = CAMERA_Y + interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar());

	position.z += incZ;

	limitZ();
}