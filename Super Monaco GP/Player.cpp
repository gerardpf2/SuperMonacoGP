#include "Player.h"

#include "Camera.h"
#include "Globals.h"
#include "ModuleInput.h"

using namespace std;

Player::Player(const WorldPosition& position, const AnimationGroup* animationGroup, const Road* road, const ModuleInput* moduleInput) :
	Car(position, animationGroup, road), moduleInput(moduleInput)
{ }

Player::~Player()
{ }

const ModuleInput* Player::getModuleInput() const
{
	return moduleInput;
}

void Player::updateDirection(float deltaTimeS)
{
	direction.x = 0.0f;
	direction.z = 0.0f;

	if(moduleInput->isKeyPressed(SDL_SCANCODE_D)) direction.x = 1.0f;
	if(moduleInput->isKeyPressed(SDL_SCANCODE_A)) direction.x = -1.0f;

	if(moduleInput->isKeyPressed(SDL_SCANCODE_W)) direction.z = 1.0f;
	if(moduleInput->isKeyPressed(SDL_SCANCODE_S)) direction.z = -1.0f;
}