#include "Animated.h"

#include "Globals.h"
#include <SDL_rect.h>
#include "Animation.h"
#include "AnimationContainer.h"

Animated::Animated(uint id, AnimationContainer* animationContainer) :
	GameObject(id), animationContainer(animationContainer)
{
	size.w = (float)animationContainer->getCurrentAnimation()->getCurrentFrame()->r->w * SPRITE_SIZE_RATIO;
	size.h = (float)animationContainer->getCurrentAnimation()->getCurrentFrame()->r->h * SPRITE_SIZE_RATIO;
}

Animated::~Animated()
{ }

GameObjectType Animated::getType() const
{
	return GameObjectType::ANIMATED;
}

const AnimationContainer* Animated::getAnimationContainer() const
{
	return animationContainer;
}

void Animated::update(float deltaTimeS)
{
	updateCurrentAnimation(deltaTimeS);
}

void Animated::cleanUp()
{
	animationContainer = nullptr;
}

void Animated::updateCurrentAnimation(float deltaTimeS) const
{
	animationContainer->update(deltaTimeS);
}

const Texture* Animated::getCurrentTexture() const
{
	return animationContainer->getCurrentAnimation()->getCurrentFrame();
}