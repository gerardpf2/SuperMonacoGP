#ifndef _ANIMATED_
#define _ANIMATED_

#include "GameObject.h"

class AnimationContainer;

class Animated : public GameObject
{
	public:

		Animated(AnimationContainer* animationContainer);

		virtual ~Animated();

		virtual GameObjectType getType() const override;

		const AnimationContainer* getAnimationContainer() const;

		virtual void update(float deltaTimeS) override;

		virtual void cleanUp() override;

	protected:

		virtual void updateCurrentAnimation(float deltaTimeS) const;

		virtual const Texture* getCurrentTexture(bool mirror) const override;

	protected:

		AnimationContainer* animationContainer = nullptr; // Constructor
};

#endif