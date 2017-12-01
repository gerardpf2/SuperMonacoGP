#include "Player.h"

#include "Camera.h"
#include "Globals.h"
#include "ModuleInput.h"

using namespace std;

Player::Player(uint id, const AnimationContainer* animationContainer, const ModuleInput* moduleInput) :
	Car(id, animationContainer), moduleInput(moduleInput)
{ }

Player::~Player()
{ }

const ModuleInput* Player::getModuleInput() const
{
	return moduleInput;
}

GameObjectType Player::getType() const
{
	return GameObjectType::PLAYER;
}

// #include <iostream>

void Player::updateDirection(float deltaTimeS)
{
	// cout << getVelocity() << endl;

	direction.x = 0.0f;
	direction.z = 0.0f;

	if(moduleInput->isKeyPressed(SDL_SCANCODE_D)) direction.x += 1.0f;
	if(moduleInput->isKeyPressed(SDL_SCANCODE_A)) direction.x -= 1.0f;

	if(moduleInput->isKeyPressed(SDL_SCANCODE_W)) direction.z += 1.0f;
	if(moduleInput->isKeyPressed(SDL_SCANCODE_S)) direction.z -= 1.0f;
}