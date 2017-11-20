#include "Player.h"

#include "Road.h"
#include "ModuleInput.h"

Player::Player(const WorldPosition& position, const Road* road, const ModuleInput* moduleInput) :
	GameObject(position, road, TextureInfo{}), moduleInput(moduleInput)
{ }

Player::~Player()
{ }

void Player::update(float deltaTimeS)
{
	// GameObject::update(deltaTimeS);

	if(moduleInput->getKeyPressed(SDL_SCANCODE_D)) moveX(deltaTimeS * 10.0f);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_A)) moveX(deltaTimeS * -10.0f);

	if(moduleInput->getKeyPressed(SDL_SCANCODE_W)) moveZ(deltaTimeS * 10.0f);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_S)) moveZ(deltaTimeS * -10.0f);
}

void Player::moveX(float incX)
{
	position.first += incX;
}

void Player::moveZ(float incZ)
{
	position.third += incZ;

	updateLimitZRoad();
}