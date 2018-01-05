#include "AnimationContainer.h"

#include <assert.h>
#include "Animation.h"

using namespace std;

AnimationContainer::AnimationContainer(uint id, uint animationGroupId, uint currentAnimationId, std::map<uint, Animation*>* animations) :
	id(id), animationGroupId(animationGroupId), currentAnimationId(currentAnimationId), animations(animations)
{ }

AnimationContainer::~AnimationContainer()
{ }

uint AnimationContainer::getId() const
{
	return id;
}

uint AnimationContainer::getAnimationGroupId() const
{
	return animationGroupId;
}

uint AnimationContainer::getCurrentAnimationId() const
{
	return currentAnimationId;
}

const map<uint, Animation*>* AnimationContainer::getAnimations() const
{
	return animations;
}

void AnimationContainer::add(Animation* animation) const
{
	assert(animation);
	assert(animations);

	(*animations)[animation->getId()] = animation;
}

Animation* AnimationContainer::getAnimation(uint animationId) const
{
	assert(animations);

	map<uint, Animation*>::iterator it = animations->find(animationId);

	assert(it != animations->end());

	return it->second;
}

Animation* AnimationContainer::getCurrentAnimation() const
{
	return getAnimation(currentAnimationId);
}

void AnimationContainer::setCurrentAnimation(uint currentAnimationId)
{
	this->currentAnimationId = currentAnimationId;
}

void AnimationContainer::update(float deltaTimeS) const
{
	Animation* animation = getCurrentAnimation();

	assert(animation);

	animation->update(deltaTimeS);
}

void AnimationContainer::cleanUp()
{
	assert(animations);

	animations->clear();

	delete animations; animations = nullptr;
}