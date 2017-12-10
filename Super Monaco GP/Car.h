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

	protected:

		Direction direction;

	private:

		float velocity = 0.0f;
};

#endif