#include "CameraFree.h"

#include "ModuleInput.h"

CameraFree::CameraFree(const Road* road, const ModuleInput* moduleInput) :
	Camera(road), moduleInput(moduleInput)
{ }

CameraFree::~CameraFree()
{ }

void CameraFree::update(float deltaTimeS)
{
	Camera::update(deltaTimeS);

	speedMultiplier = moduleInput->getKeyPressed(SDL_SCANCODE_SPACE) ? 5.0f : 1.0f;

	if(moduleInput->getKeyPressed(SDL_SCANCODE_W)) moveZ(deltaTimeS * speedMultiplier * CAMERA_FREE_SPEED_Z);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_S)) moveZ(deltaTimeS * speedMultiplier * -CAMERA_FREE_SPEED_Z);
}

void CameraFree::moveZ(float incZ)
{
	position.third += incZ;

	updateLimitZRoad();
}