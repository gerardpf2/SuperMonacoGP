#include "AnimationContainer.h"

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

void AnimationContainer::add(Animation* animation)
{
	(*animations)[animation->getId()] = animation;
}

Animation* AnimationContainer::getAnimation(uint animationId) const
{
	return (*animations)[animationId];
}

Animation* AnimationContainer::getCurrentAnimation() const
{
	return animations->at(currentAnimationId);
}

void AnimationContainer::setCurrentAnimation(uint currentAnimationId)
{
	this->currentAnimationId = currentAnimationId;
}

void AnimationContainer::update(float deltaTimeS) const
{
	getCurrentAnimation()->update(deltaTimeS);
}

void AnimationContainer::cleanUp()
{
	animations->clear();

	delete animations; animations = nullptr;
}