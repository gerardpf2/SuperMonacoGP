#include "Player.h"

#include "Utils.h"
#include "Animation.h"
#include "ModuleInput.h"
#include "AnimationGrid.h"
#include "AnimationContainer.h"

Player::Player(AnimationContainer* animationContainer, const ModuleInput* moduleInput) :
	Car(animationContainer), moduleInput(moduleInput)
{
	assert(animationContainer);

	animationGrid = new AnimationGrid(animationContainer->getAnimation(0));

	animationGrid->addRightV(animationContainer->getAnimation(1));
	animationGrid->addRightV(animationContainer->getAnimation(3));
	animationGrid->addRightV(animationContainer->getAnimation(5));

	animationGrid->addLeftV(animationContainer->getAnimation(2));
	animationGrid->addLeftV(animationContainer->getAnimation(4));
	animationGrid->addLeftV(animationContainer->getAnimation(6));

	animationGrid->addRightH(animationContainer->getAnimation(7), animationContainer->getAnimation(8));
	animationGrid->addRightH(animationContainer->getAnimation(11), animationContainer->getAnimation(12));
	animationGrid->addRightH(animationContainer->getAnimation(15), animationContainer->getAnimation(16));

	animationGrid->addLeftH(animationContainer->getAnimation(9), animationContainer->getAnimation(10));
	animationGrid->addLeftH(animationContainer->getAnimation(13), animationContainer->getAnimation(14));
	animationGrid->addLeftH(animationContainer->getAnimation(17), animationContainer->getAnimation(18));
}

Player::~Player()
{ }

GameObjectType Player::getType() const
{
	return GameObjectType::PLAYER;
}

const ModuleInput* Player::getModuleInput() const
{
	return moduleInput;
}

void Player::cleanUp()
{
	assert(animationGrid);

	animationGrid->cleanUp();

	delete animationGrid; animationGrid = nullptr;
}

void Player::updateDirection(float deltaTimeS)
{
	assert(moduleInput);

	direction.x = 0.0f;
	direction.z = 0.0f;

	if(moduleInput->isKeyPressed(SDL_SCANCODE_D) || moduleInput->isKeyPressed(SDL_SCANCODE_RIGHT)) direction.x += 1.0f;
	if(moduleInput->isKeyPressed(SDL_SCANCODE_A) || moduleInput->isKeyPressed(SDL_SCANCODE_LEFT)) direction.x -= 1.0f;

	if(moduleInput->isKeyPressed(SDL_SCANCODE_W) || moduleInput->isKeyPressed(SDL_SCANCODE_UP)) direction.z += 1.0f;
	if(moduleInput->isKeyPressed(SDL_SCANCODE_S) || moduleInput->isKeyPressed(SDL_SCANCODE_DOWN)) direction.z -= 1.0f;
}

void Player::updateVelocityCurve(float deltaTimeS)
{
	// Reduce the velocity because of the direction x

	float multiplier = getVelocityPercent() * interpolate(fabsf(direction.x), 0.0f, 1.0f, 0.0f, 1.0f);
	velocity -= multiplier * deltaTimeS;
}

void Player::updateCurrentAnimation(float deltaTimeS) const
{
	assert(animationGrid);

	animationGrid->advance(getVelocityPercent(), direction.x, deltaTimeS);

	Animation* animation = animationGrid->getCurrentAnimation();

	assert(animation);
	assert(animationContainer);

	animationContainer->setCurrentAnimation(animation->getId());
}

void Player::updateOffsetX(float dX, float velocityPercent, float curve)
{
	position.x += 4.0f * ROAD_WIDTH * dX * velocityPercent * -curve;
}

const Texture* Player::getCurrentTexture(bool mirror) const
{
	return Animated::getCurrentTexture(mirror);
}