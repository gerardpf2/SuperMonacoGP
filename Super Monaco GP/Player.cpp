#include "Player.h"

#include "ModuleInput.h"

Player::Player(const WorldPosition& position, const Road* road, const ModuleInput* moduleInput, Texture* texture) :
	Car(position, road, texture), moduleInput(moduleInput)
{
	maxVelocity = 60.0f; //
}

Player::~Player()
{ }

void Player::updateDirections(float deltaTimeS)
{
	directionX = 0.0f;
	directionZ = 0.0f;

	if(moduleInput->getKeyPressed(SDL_SCANCODE_D)) directionX = 1.0f;
	if(moduleInput->getKeyPressed(SDL_SCANCODE_A)) directionX = -1.0f;

	if(moduleInput->getKeyPressed(SDL_SCANCODE_W)) directionZ = 1.0f;
	if(moduleInput->getKeyPressed(SDL_SCANCODE_S)) directionZ = -1.0f;
}