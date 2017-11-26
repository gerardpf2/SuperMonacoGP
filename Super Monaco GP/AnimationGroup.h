#ifndef _ANIMATION_GROUP_
#define _ANIMATION_GROUP_

#include <map>
#include "rapidjson/document.h"

class Animation;
class ModuleAnimation;
enum class AnimationType;
enum class AnimationGroupType;

class AnimationGroup
{
	public:

		AnimationGroup(const AnimationGroup& animationGroup, ModuleAnimation* moduleAnimation);
		AnimationGroup(const rapidjson::Value& value, ModuleAnimation* moduleAnimation);

		virtual ~AnimationGroup();

		AnimationGroupType getType() const;

		AnimationType getCurrentType() const;

		const std::map<AnimationType, Animation*>* getAnimations() const;

		void add(Animation* animation);

		Animation* getCurrent() const;

		void setCurrent(AnimationType currentType);

		void update(float deltaTimeS) const;

		void cleanUp();

	private:

		AnimationGroupType type;

		AnimationType currentType;

		std::map<AnimationType, Animation*> animations;
};

#endif