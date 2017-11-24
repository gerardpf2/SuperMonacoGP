#ifndef _CAR_
#define _CAR_

#include "GameObject.h"

class Car : public GameObject
{
	public:

		Car(const WorldPosition& position, const Road* road, Texture* texture);

		virtual ~Car();

		virtual void update(float deltaTimeS) override;

	protected:

		virtual void updateDirections(float deltaTimeS);

	protected:

		float acceleration = 10.0f;

		float deacceleration = acceleration;

		float velocity = 0.0f;

		float maxVelocity = 50.0f; // 50.0f;

		float directionX = 0.0f;

		float directionZ = 1.0f;
};

#endif