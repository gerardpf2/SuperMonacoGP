#include "Car.h"

#include "Road.h"
#include "Utils.h"
#include "Camera.h"
#include "Segment.h"
#include "Animation.h"
#include "GameEngine.h"
#include "ModuleWorld.h"
#include "ModuleRegistry.h"
#include "ModuleCollision.h"
#include "AnimationContainer.h"

using namespace std;

Car::Car(/* uint id, */ AnimationContainer* animationContainer) :
	Animated(/* id, */ animationContainer)
{ }

Car::~Car()
{ }

GameObjectType Car::getType() const
{
	return GameObjectType::CAR;
}

uint Car::getSpecificId() const
{
	return specificId;
}

void Car::setSpecificId(uint specificId)
{
	this->specificId = specificId;
}

float Car::getVelocity() const
{
	return velocity;
}

float Car::getVelocityPercent() const
{
	return velocity / CAR_MAX_VELOCITY;
}

uint Car::getCurrentLap() const
{
	return currentLap;
}

float Car::getCurrentLapTime() const
{
	return currentLapTime;
}

float Car::getCurrentLapTimeStored() const
{
	return currentLapTimeStored;
}

bool Car::getMovementEnabled() const
{
	return movementEnabled;
}

void Car::setMovementEnabled(bool movementEnabled)
{
	this->movementEnabled = movementEnabled;
}

void Car::update(float deltaTimeS)
{
	Animated::update(deltaTimeS);

	float maxVelocity = CAR_MAX_VELOCITY;
	float acceleration = CAR_ACCELERATION_ROAD;
	float deacceleration = CAR_DEACCELERATION_ROAD;
	float deaccelerationFriction = CAR_DEACCELERATION_FRICTION_ROAD;
	float deaccelerationFrictionExtra = CAR_DEACCELERATION_FRICTION_EXTRA_ROAD;

	if(position.x < ROAD_MIN_X || position.x > ROAD_MAX_X)
	{
		acceleration = CAR_ACCELERATION_GRASS;
		deacceleration = CAR_DEACCELERATION_GRASS;
		deaccelerationFriction = CAR_DEACCELERATION_FRICTION_GRASS;
		deaccelerationFrictionExtra = CAR_DEACCELERATION_FRICTION_EXTRA_GRASS;
	}

	updateDirection(deltaTimeS);
	// updateCurrentAnimation(deltaTimeS);

	Segment* segment = getModuleWorld()->getRoad()->getSegmentAtZ(position.z);

	position.z += velocity * deltaTimeS;
	
	globalZ += velocity * deltaTimeS;

	limitZ();

	Segment* newSegment = getModuleWorld()->getRoad()->getSegmentAtZ(position.z);

	float velocityPercent = getVelocityPercent();

	// animationContainer->getCurrentAnimation()->setTimeMultiplier(0.0f);
	// animationContainer->getCurrentAnimation()->advancePercent(2.0f * speedPercent);

	// animationContainer->getCurrentAnimation()->setTimeMultiplier(0.0f);
	// animationContainer->getCurrentAnimation()->setTimeMultiplier(speedPercent);

	float dX = 0.75f * ROAD_WIDTH * velocityPercent * deltaTimeS;

	position.x += direction.x * dX;

	updateOffsetX(dX, velocityPercent, newSegment->getCurve());

	// position.x = clamp(position.x, CAR_MIN_X, CAR_MAX_X);

	position.y = interpolate(position.z, newSegment->getZNear(), newSegment->getZFar(), newSegment->getYNear(), newSegment->getYFar());

	acceleration *= (1.0f - powf(velocityPercent, 2.0f));

	velocity -= deaccelerationFriction * deltaTimeS;

	// direction.z = 1.0f; //

	if(movementEnabled)
	{
		if(direction.z == 1.0f) velocity += acceleration * deltaTimeS;
		else if(direction.z == -1.0f) velocity -= deacceleration * deltaTimeS;
	}

	if(velocity > CAR_DEACCELERATION_FRICTION_LIMIT_VELOCITY)
		velocity -= deaccelerationFrictionExtra * deltaTimeS;

	velocity = clamp(velocity, 0.0f, maxVelocity);

	if(segment != newSegment)
	{
		segment->eraseGameObject(this);
		newSegment->addGameObject(this);
	}

	checkCollision();

	position.x = clamp(position.x, CAR_MIN_X, CAR_MAX_X);

	if(movementEnabled) currentLapTime += deltaTimeS;

	if(lapCompleted())
	{
		if(!ignoreCurrentLap)
		{
			registerLapTime();

			++currentLap;
			currentLapTimeStored = currentLapTime;
			currentLapTime = 0.0f;
		}
		else ignoreCurrentLap = false;
	}
}

void Car::updateDirection(float deltaTimeS)
{
	direction.x = 0.0f;
	direction.z = 1.0f;
}

void Car::updateCurrentAnimation(float deltaTimeS) const
{
	animationContainer->getCurrentAnimation()->setTimeMultiplier(getVelocityPercent());

	Animated::updateCurrentAnimation(deltaTimeS);

	/*
	
	AnimationId 0 -> Center
	AnimationId 1 -> Camera right side
	AnimationId 2 -> Camera left side
	AnimationId 3 -> Camera right side +
	AnimationId 4 -> Camera left side +
	
	*/

	Animation* currentAnimation = animationContainer->getCurrentAnimation();
	Animation* nextAnimation = currentAnimation;

	float x = position.x;
	float cameraX = getModuleWorld()->getCamera()->getPosition()->x;

	x -= getModuleWorld()->getRoad()->getSegmentAtZ(position.z)->getXOffsetNear();
	cameraX -= getModuleWorld()->getRoad()->getSegmentAtZ(getModuleWorld()->getCamera()->getBasePositionZ())->getXOffsetNear();

	float threshold0 = 5.0f;
	float threshold1 = 20.0f;
	float cameraDistance = cameraX - x;
	float cameraDistanceAbs = fabsf(cameraDistance);

	if(cameraDistanceAbs <= threshold0)
		nextAnimation = animationContainer->getAnimation(0);
	else if(cameraDistanceAbs <= threshold1)
		nextAnimation = animationContainer->getAnimation(cameraDistance > 0.0f ? 1 : 2);
	else
		nextAnimation = animationContainer->getAnimation(cameraDistance > 0.0f ? 3 : 4);

	if(nextAnimation != currentAnimation)
	{
		nextAnimation->synchronize(*animationContainer->getCurrentAnimation());
		animationContainer->setCurrentAnimation(nextAnimation->getId());
	}
}

void Car::updateOffsetX(float dX, float velocityPercent, float curve)
{ }

const Texture* Car::getCurrentTexture(bool mirror) const
{
	Animation* currentAnimation = animationContainer->getCurrentAnimation();
	const Texture* currentFrame = currentAnimation->getCurrentFrame();

	if(mirror)
	{
		Animation* mirrorAnimation = nullptr;

		switch(currentAnimation->getId())
		{
			case 0:
				mirrorAnimation = animationContainer->getAnimation(5);

				break;
			case 1:
			case 3:
				mirrorAnimation = animationContainer->getAnimation(6);

				break;
			case 2:
			case 4:
				mirrorAnimation = animationContainer->getAnimation(7);

				break;
		}

		if(mirrorAnimation) currentFrame = mirrorAnimation->getFrame(currentAnimation->getCurrentFrameIndex());
	}

	return currentFrame;
}

void Car::checkCollision()
{
	/*
	
	Priority: Static = Animated > Car = Player

	Find biggest static / animated
	If found, solve collision and end

	Find first car / player
	If found, solve collision
	
	*/

	const Collider* mainCollider = nullptr;
	const list<const Collider*>* colliding = nullptr;
	
	if(getModuleWorld()->getGameEngine()->getModuleCollision()->getColliding(&collider, colliding))
	{
		for(list<const Collider*>::const_iterator it = colliding->begin(); it != colliding->end(); ++it)
		{
			const Collider* currentCollider = *it;

			// float z0 = mod0L(position.z + collider.b.d, getModuleWorld()->getRoad()->getLength());
			// float z1 = mod0L(currentCollider->g->getPosition()->z + currentCollider->b.d, getModuleWorld()->getRoad()->getLength());

			float z0 = position.z + collider.b.d; // mod0L(position.z + collider.b.d, getModuleWorld()->getRoad()->getLength());
			float z1 = currentCollider->g->getPosition()->z + currentCollider->b.d; // mod0L(currentCollider->g->getPosition()->z + currentCollider->b.d, getModuleWorld()->getRoad()->getLength());

			if(z1 < z0) continue;
			// if(currentCollider->g->getPosition()->z < position.z) continue;

			if(!mainCollider) mainCollider = currentCollider;
			else
			{
				switch(mainCollider->g->getType())
				{
					case GameObjectType::STATIC:
					case GameObjectType::ANIMATED:
						if(currentCollider->g->getType() == GameObjectType::STATIC || currentCollider->g->getType() == GameObjectType::ANIMATED)
							if(currentCollider->b.w > mainCollider->b.w)
								mainCollider = currentCollider;

						break;
					case GameObjectType::CAR:
					case GameObjectType::PLAYER:
						if(currentCollider->g->getType() == GameObjectType::STATIC || currentCollider->g->getType() == GameObjectType::ANIMATED)
							mainCollider = currentCollider;
				}
			}
		}
	}

	if(mainCollider)
	{
		velocity *= 0.5f;
		position.z -= /* getVelocityPercent() * */ SEGMENT_LENGTH; limitZ();

		globalZ -= SEGMENT_LENGTH;

		if(mainCollider->g->getType() == GameObjectType::STATIC || mainCollider->g->getType() == GameObjectType::ANIMATED)
		{
			float maxW = fmaxf(mainCollider->b.w, collider.b.w);

			if(mainCollider->g->getPosition()->x >= ROAD_MAX_X)
				position.x = ROAD_MAX_X - maxW;
			else if(mainCollider->g->getPosition()->x <= ROAD_MIN_X)
				position.x = ROAD_MIN_X + maxW;
			else if(mainCollider->g->getPosition()->x >= position.x)
				position.x -= mainCollider->b.w;
			else
				position.x += mainCollider->b.w;
		}
	}
}

void Car::registerLapTime() const
{
	getModuleWorld()->registerLapTime(this);
}

bool Car::lapCompleted()
{
	// bool previousCountingLap = countingLap;

	float z0 = (1.0f / 3.0f) * getModuleWorld()->getRoad()->getLength();
	float z1 = (2.0f / 3.0f) * getModuleWorld()->getRoad()->getLength();

	if(countingLap && position.z < z0)
	{
		countingLap = false;

		return true;
	}

	if(position.z >= z0 && position.z <= z1)
		countingLap = true;

	return false;

	// return previousCountingLap != countingLap && previousCountingLap;
}