#include "AnimationContainer.h"

#include "Animation.h"

using namespace std;

AnimationContainer::AnimationContainer(uint id, uint currentAnimationId, std::map<uint, Animation*>* animations) :
	id(id), currentAnimationId(currentAnimationId), animations(animations)
{ }

/* AnimationContainer::AnimationContainer(const AnimationContainer& animationGroup, ModuleAnimation* moduleAnimation) :
	id(animationGroup.id), currentId(animationGroup.currentId)
{
	for(map<uint, Animation*>::const_iterator it = animationGroup.animations.begin(); it != animationGroup.animations.end(); ++it)
		animations[it->first] = moduleAnimation->getAnimationCopy(it->first);
} */

AnimationContainer::~AnimationContainer()
{ }

uint AnimationContainer::getId() const
{
	return id;
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
	/* for(map<uint, Animation*>::reverse_iterator it = animations->rbegin(); it != animations->rend(); ++it)
	{
		it->second->cleanUp();

		delete it->second; it->second = nullptr;
	} */

	animations->clear();

	delete animations; animations = nullptr;
}