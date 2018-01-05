#include "Animated.h"

#include <assert.h>
#include "Globals.h"
#include <SDL_rect.h>
#include "Animation.h"
#include "AnimationContainer.h"

Animated::Animated(AnimationContainer* animationContainer) :
	animationContainer(animationContainer)
{
	assert(animationContainer);

	Animation* animation = animationContainer->getCurrentAnimation();

	assert(animation);

	const Texture* texture = animation->getCurrentFrame();

	assert(texture);
	assert(texture->r);

	size.w = (float)texture->r->w * SPRITE_SIZE_RATIO;
	size.h = (float)texture->r->h * SPRITE_SIZE_RATIO;
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
	assert(animationContainer);

	animationContainer->update(deltaTimeS);
}

const Texture* Animated::getCurrentTexture(bool mirror) const
{
	assert(animationContainer);

	Animation* animation = animationContainer->getCurrentAnimation();

	assert(animation);

	return animation->getCurrentFrame();
}