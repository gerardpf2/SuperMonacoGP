#ifndef _CAR_
#define _CAR_

#include "GameObject.h"

class Car : public GameObject
{
	public:

		Car(uint id, const AnimationContainer* animationContainer);

		virtual ~Car();

		virtual GameObjectType getType() const override;

		float getVelocity() const;

		virtual void update(float deltaTimeS) override;

	protected:

		virtual void updateDirection(float deltaTimeS);

	protected:

		Direction direction;

	private:

		float velocity = 0.0f;
};

#endif