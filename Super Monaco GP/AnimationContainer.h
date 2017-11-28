#ifndef _ANIMATION_CONTAINER_
#define _ANIMATION_CONTAINER_

#include <map>
#include "Types.h"

class Animation;

class AnimationContainer
{
	public:

		AnimationContainer(uint id, uint currentAnimationId, std::map<uint, Animation*>* animations);

		// AnimationContainer(const AnimationContainer& animationGroup, ModuleAnimation* moduleAnimation);

		virtual ~AnimationContainer();

		uint getId() const;

		uint getCurrentAnimationId() const;

		const std::map<uint, Animation*>* getAnimations() const;

		void add(Animation* animation);

		Animation* getCurrentAnimation() const;
		
		void setCurrentAnimation(uint currentAnimationId);

		void update(float deltaTimeS) const;

		void cleanUp();

	private:

		uint id;

		uint currentAnimationId;

		std::map<uint, Animation*>* animations;
};

#endif