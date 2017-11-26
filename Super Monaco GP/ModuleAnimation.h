#ifndef _MODULE_ANIMATION_
#define _MODULE_ANIMATION_

#include <list>
#include <vector>
#include "Module.h"
#include "rapidjson/document.h"

enum class AnimationType
{
	PLAYER_FORWARD,
};

enum class AnimationGroupType
{
	PLAYER,
};

struct Texture;
class Animation;
class AnimationGroup;

class ModuleAnimation : public Module
{
	public:

		ModuleAnimation(GameEngine* gameEngine);

		virtual ~ModuleAnimation();

		Animation* getAnimationCopy(AnimationType animationType);

		AnimationGroup* getAnimationGroupCopy(AnimationGroupType animationGroupType);

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		void load(const rapidjson::Document& document);

	private:

		std::vector<Animation*> animations;

		std::vector<AnimationGroup*> animationGroups;

		std::list<Animation*> usedAnimations;

		std::list<AnimationGroup*> usedAnimationGroups;
};

#endif