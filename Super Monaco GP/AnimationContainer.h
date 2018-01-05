#ifndef _ANIMATION_CONTAINER_
#define _ANIMATION_CONTAINER_

#include <map>
#include "Types.h"

class Animation;

class AnimationContainer
{
	public:

		AnimationContainer(uint id, uint animationGroupId, uint currentAnimationId, std::map<uint, Animation*>* animations);

		virtual ~AnimationContainer();

		uint getId() const;

		uint getAnimationGroupId() const;

		uint getCurrentAnimationId() const;

		const std::map<uint, Animation*>* getAnimations() const;

		void add(Animation* animation) const;

		Animation* getAnimation(uint animationId) const;

		Animation* getCurrentAnimation() const;
		
		void setCurrentAnimation(uint currentAnimationId);

		void update(float deltaTimeS) const;

		void cleanUp();

	private:

		uint id; // Constructor

		uint animationGroupId; // Constructor

		uint currentAnimationId; // Constructor

		std::map<uint, Animation*>* animations = nullptr; // Constructor
};

#endif