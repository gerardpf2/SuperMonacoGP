#include "CameraFree.h"

#include "ModuleInput.h"

CameraFree::CameraFree(const Road* road, const ModuleInput* moduleInput) :
	Camera(road), moduleInput(moduleInput)
{ }

CameraFree::~CameraFree()
{ }

void CameraFree::update(float deltaTimeS)
{
	if(moduleInput->getKeyPressed(SDL_SCANCODE_Q)) moveY(deltaTimeS * CAMERA_FREE_SPEED_Y);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_E)) moveY(deltaTimeS * -CAMERA_FREE_SPEED_Y);
	
	if(moduleInput->getKeyPressed(SDL_SCANCODE_W)) moveZ(deltaTimeS * CAMERA_FREE_SPEED_Z);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_S)) moveZ(deltaTimeS * -CAMERA_FREE_SPEED_Z);
}

void CameraFree::moveY(float incY)
{
	float positionY = position.y;

	position.y = fminf(fmaxf(position.y + incY, CAMERA_MIN_Y), CAMERA_MAX_Y);
	position.z -= (position.y - positionY) * depth;
}

void CameraFree::moveZ(float incZ)
{
	position.z += incZ;

	updateLimitZRoad();
}