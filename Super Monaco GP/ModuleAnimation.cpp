#include "ModuleAnimation.h"

#include "Animation.h"
#include "GameEngine.h"
#include "ModuleJson.h"
#include "ModuleTexture.h"
#include "AnimationGroup.h"

using namespace std;
using namespace rapidjson;

ModuleAnimation::ModuleAnimation(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleAnimation::~ModuleAnimation()
{ }

Animation* ModuleAnimation::getAnimationCopy(AnimationType animationType)
{
	Animation* animationCopy = new Animation(*animations[(uint)animationType]);
	usedAnimations.push_back(animationCopy);

	return animationCopy;
}

AnimationGroup* ModuleAnimation::getAnimationGroupCopy(AnimationGroupType animationGroupType)
{
	AnimationGroup* animationGroupCopy = new AnimationGroup(*animationGroups[(uint)animationGroupType], this);
	usedAnimationGroups.push_back(animationGroupCopy);

	return animationGroupCopy;
}

bool ModuleAnimation::setUp()
{
	Document animationsDocument;
	getGameEngine()->getModuleJson()->read("Resources/Configuration/Animations.json", animationsDocument);

	load(animationsDocument);

	return true;
}

void ModuleAnimation::cleanUp()
{
	for(int i = (int)animations.size() - 1; i >= 0; --i)
	{
		animations[i]->cleanUp(true);

		delete animations[i];
		animations[i] = nullptr;
	}

	animations.clear();

	for(list<Animation*>::reverse_iterator it = usedAnimations.rbegin(); it != usedAnimations.rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it;
		*it = nullptr;
	}

	usedAnimations.clear();

	for(int i = (int)animationGroups.size() - 1; i >= 0; --i)
	{
		animationGroups[i]->cleanUp();

		delete animationGroups[i];
		animationGroups[i] = nullptr;
	}

	animationGroups.clear();

	for(list<AnimationGroup*>::reverse_iterator it = usedAnimationGroups.rbegin(); it != usedAnimationGroups.rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it;
		*it = nullptr;
	}

	usedAnimationGroups.clear();
}

void ModuleAnimation::load(const Document& document)
{
	const ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	// Animations
	// Animation -> Type, Loop, End time, Texture types

	const Value& animationsJson = document["animations"];

	animations.resize(animationsJson.Size(), nullptr);

	for(SizeType i = 0; i < animationsJson.Size(); ++i)
	{
		Animation* animation = new Animation(animationsJson[i], moduleTexture);
		animations[(uint)animation->getType()] = animation;
	}

	// AnimationGroups
	// AnimationGroup -> Type, Initial animation type, List animation types

	const Value& animationGroupsJson = document["animationGroups"];

	animationGroups.resize(animationGroupsJson.Size(), nullptr);

	for(SizeType i = 0; i < animationGroupsJson.Size(); ++i)
	{
		AnimationGroup* animationGroup = new AnimationGroup(animationGroupsJson[i], this);
		animationGroups[(uint)animationGroup->getType()] = animationGroup;
	}
}