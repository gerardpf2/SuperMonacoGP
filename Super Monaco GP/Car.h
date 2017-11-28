#ifndef _CAR_
#define _CAR_

#include "GameObject.h"

class Car : public GameObject
{
	public:

		Car(const WorldPosition& position, const AnimationContainer* animationContainer, const Road* road);

		virtual ~Car();

		virtual void update(float deltaTimeS) override;

	protected:

		virtual void updateDirection(float deltaTimeS);

	protected:

		Direction direction;

	private:

		float velocity = 0.0f;
};

#endif