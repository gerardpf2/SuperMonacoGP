#ifndef _CAR_
#define _CAR_

#include "Animated.h"

class Car : public Animated
{
	public:

		Car(/* uint id, */ AnimationContainer* animationContainer);

		virtual ~Car();

		virtual GameObjectType getType() const override;

		virtual void setPosition(const WorldPosition& position) override;

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

		float initialX;

		// float changeDirectionXTime = 10.0f;

		// float changeDirectionXCurrentTime = 0.0f;

		// float desiredDirectionX = 0.0f;
};

#endif