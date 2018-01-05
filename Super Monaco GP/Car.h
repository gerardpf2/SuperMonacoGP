#ifndef _CAR_
#define _CAR_

#define DIRECTION_Z_FAR 30.0f
#define DIRECTION_Z_CLOSE 7.5f

#define MIN_VELOCITY_MULTIPLIER 0.7f
#define MAX_VELOCITY_MULTIPLIER 1.0f

#define MIN_ACCELERATION_MULTIPLIER 0.8f
#define MAX_ACCELERATION_MULTIPLIER 1.0f

#include "Animated.h"

class Car : public Animated
{
	public:

		Car(AnimationContainer* animationContainer);

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

		virtual void updateVelocityCurve(float deltaTimeS);

		virtual void updateCurrentAnimation(float deltaTimeS) const override;

		virtual void updateOffsetX(float dX, float velocityPercent, float curve);

		virtual const Texture* getCurrentTexture(bool mirror) const override;

	private:

		void computeVelocityAccelerationMultipliersMultiplier();

		void checkCollision();

		bool lapCompleted();

		void registerLapTime() const;

	protected:

		Direction direction;

		float velocity = 0.0f;

	private:

		uint specificId = 0;

		uint currentLap = 0;

		float currentLapTime = 0.0f;

		float currentLapTimeStored = 0.0f;

		bool countingLap = true;

		bool ignoreCurrentLap = true;

		bool movementEnabled = false;

		float velocityMultiplier = 1.0f;

		float accelerationMultiplier = 1.0f;
};

#endif