#ifndef _CAR_
#define _CAR_

#include "Animated.h"

class Car : public Animated
{
	public:

		Car(/* uint id, */ AnimationContainer* animationContainer);

		virtual ~Car();

		virtual GameObjectType getType() const override;

		uint getSpecificId() const;

		void setSpecificId(uint specificId);

		float getVelocity() const;

		float getVelocityPercent() const;

		uint getCurrentLap() const;

		float getCurrentLapTime() const;

		float getCurrentLapTimeStored() const;

		bool getMovementEnabled() const;

		void setMovementEnabled(bool movementEnabled);

		virtual void update(float deltaTimeS) override;

	protected:

		virtual void updateDirection(float deltaTimeS);

		virtual void updateCurrentAnimation(float deltaTimeS) const override;

		virtual void updateOffsetX(float dX, float velocityPercent, float curve);

		virtual const Texture* getCurrentTexture(bool mirror) const override;

	private:

		void checkCollision();

		bool lapCompleted();

		void registerLapTime() const;

	protected:

		Direction direction;

	private:

		uint specificId = 0;

		float velocity = 0.0f;

		uint currentLap = 0;

		float currentLapTime = 0.0f;

		float currentLapTimeStored = 0.0f;

		bool countingLap = true;

		bool ignoreCurrentLap = true;

		bool movementEnabled = false;
};

#endif