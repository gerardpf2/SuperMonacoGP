#ifndef _ANIMATED_
#define _ANIMATED_

#include "GameObject.h"

class AnimationContainer;

class Animated : public GameObject
{
	public:

		Animated(uint id, const AnimationContainer* animationContainer);

		virtual ~Animated();

		virtual GameObjectType getType() const override;

		const AnimationContainer* getAnimationContainer() const;

		virtual void update(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		virtual const Texture* getCurrentTexture() const override;

	protected:

		const AnimationContainer* animationContainer = nullptr;
};

#endif