#ifndef _CAR_
#define _CAR_

#include "Animated.h"

class Car : public Animated
{
	public:

		Car(uint id, AnimationContainer* animationContainer);

		virtual ~Car();

		virtual GameObjectType getType() const override;

		float getVelocity() const;

		float getVelocityPercent() const;

		virtual void update(float deltaTimeS) override;

	protected:

		virtual void updateDirection(float deltaTimeS);

		virtual void updateCurrentAnimation(float deltaTimeS) const override;

		virtual void updateOffsetX(float dX, float velocityPercent, float curve);

		virtual const Texture* getCurrentTexture(bool mirror) const override;

	private:

		void checkCollision();

	protected:

		Direction direction;

	private:

		float velocity = 0.0f;
};

#endif