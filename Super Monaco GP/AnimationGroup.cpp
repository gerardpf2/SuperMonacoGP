#include "AnimationGroup.h"

#include "Animation.h"
#include "ModuleAnimation.h"

using namespace std;
using namespace rapidjson;

AnimationGroup::AnimationGroup(const AnimationGroup& animationGroup, ModuleAnimation* moduleAnimation) :
	type(animationGroup.type), currentType(animationGroup.currentType)
{
	for(map<AnimationType, Animation*>::const_iterator it = animationGroup.animations.begin(); it != animationGroup.animations.end(); ++it)
		animations[it->first] = moduleAnimation->getAnimationCopy(it->first);
}

AnimationGroup::AnimationGroup(const Value& value, ModuleAnimation* moduleAnimation)
{
	type = (AnimationGroupType)value["type"].GetUint();
	currentType = (AnimationType)value["current"].GetUint();

	const Value& animationsJson = value["animations"];

	for(SizeType i = 0; i < animationsJson.Size(); ++i)
	{
		AnimationType animationType = (AnimationType)animationsJson[i].GetUint();
		animations[animationType] = moduleAnimation->getAnimationCopy(animationType);
	}
}

AnimationGroup::~AnimationGroup()
{ }

AnimationGroupType AnimationGroup::getType() const
{
	return type;
}

AnimationType AnimationGroup::getCurrentType() const
{
	return currentType;
}

const map<AnimationType, Animation*>* AnimationGroup::getAnimations() const
{
	return &animations;
}

void AnimationGroup::add(Animation* animation)
{
	animations[animation->getType()] = animation;
}

Animation* AnimationGroup::getCurrent() const
{
	return animations.at(currentType);
}

void AnimationGroup::setCurrent(AnimationType currentType)
{
	this->currentType = currentType;
}

void AnimationGroup::update(float deltaTimeS) const
{
	getCurrent()->update(deltaTimeS);
}

#include <iostream>
using namespace std;

void AnimationGroup::cleanUp()
{
	animations.clear();
}