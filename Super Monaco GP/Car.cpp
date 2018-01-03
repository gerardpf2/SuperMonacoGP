#include "Car.h"

#include "Road.h"
#include "Utils.h"
#include "Camera.h"
#include "Player.h"
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

/* void Car::setPosition(const WorldPosition& position)
{
	GameObject::setPosition(position);

	initialX = position.x;
} */

uint Car::getSpecificId() const
{
	return specificId;
}

void Car::setSpecificId(uint specificId)
{
	this->specificId = specificId;

	computeVelocityAccelerationMultipliersMultiplier();
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

		if(getType() == GameObjectType::PLAYER && velocity > 0.0f)
			getModuleWorld()->playFxOutRoad();
	}

	maxVelocity *= velocityMultiplier;
	acceleration *= accelerationMultiplier;

	updateDirection(deltaTimeS);
	
	// updateVelocityCurve(deltaTimeS);
	
	// updateCurrentAnimation(deltaTimeS);

	Segment* segment = getModuleWorld()->getRoad()->getSegmentAtZ(position.z);

	position.z += velocity * deltaTimeS;
	
	globalZ += velocity * deltaTimeS;

	checkCollision();

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

	updateVelocityCurve(deltaTimeS);

	velocity = clamp(velocity, 0.0f, maxVelocity);

	if(segment != newSegment)
	{
		segment->eraseGameObject(this);
		newSegment->addGameObject(this);
	}

	// checkCollision();

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
	const float farZ = 30.0f; //
	const float closeZ = 7.5f; //

	direction.x = 0.0f;
	direction.z = 1.0f;

	float w = box.w;
	float x = position.x;
	float collisionLeftX = x - 0.5f * w;
	float collisionRightX = x + 0.5f * w;

	list<const GameObject*> obstaclesFront;
	getModuleWorld()->getRoad()->findGameObjectsFront(position.z, farZ, obstaclesFront);

	for(const GameObject* obstacle : obstaclesFront)
	{
		if(obstacle == this) continue;

		if(obstacle->getType() == GameObjectType::CAR || obstacle->getType() == GameObjectType::PLAYER)
			if(((Car*)obstacle)->getVelocityPercent() > getVelocityPercent()) continue;

		float obstacleW = obstacle->getBox()->w;
		float obstacleX = obstacle->getPosition()->x;
		float obstacleCollisionLeftX = obstacleX - 0.75f * obstacleW;
		float obstacleCollisionRightX = obstacleX + 0.75f * obstacleW;

		bool mayCollide = !(obstacleCollisionLeftX > collisionRightX || obstacleCollisionRightX < collisionLeftX);

		if(mayCollide)
		{
			float remainingSpaceObstacleLeftX = fabsf(ROAD_MIN_X - obstacleCollisionLeftX);
			float remainingSpaceObstacleRightX = fabsf(ROAD_MAX_X - obstacleCollisionRightX);

			float diffZ = fabsf(obstacle->getPosition()->z - position.z);
			float directionLeftX = interpolate(diffZ, 0.0f, farZ, -1.0f, 0.0f);
			float directionRightX = interpolate(diffZ, 0.0f, farZ, 1.0f, 0.0f);

			if(x <= obstacleX && remainingSpaceObstacleLeftX > w) direction.x = directionLeftX;
			else if(x > obstacleX && remainingSpaceObstacleRightX > w) direction.x = directionRightX;
			else
			{
				if(remainingSpaceObstacleLeftX >= remainingSpaceObstacleRightX)
					direction.x = directionLeftX;
				else
					direction.x = directionRightX;
			}
			
			if(diffZ < closeZ) direction.z = -1.0;

			break;
		}
	}

	direction.x = clamp(direction.x, -1.0f, 1.0f);
	direction.z = clamp(direction.z, -1.0f, 1.0f);
}

void Car::updateVelocityCurve(float deltaTimeS)
{
	Segment* segment = getModuleWorld()->getRoad()->getSegmentAtZ(position.z);
	
	float curve = segment->getCurve();

	float multiplier = interpolate(fabsf(curve), 0.0f, 0.02f, 0.0f, 10.0f);

	velocity -= multiplier * deltaTimeS;

	float multiplier2 = getVelocityPercent() * interpolate(fabsf(direction.x), 0.0f, 1.0f, 0.0f, 2.0f);

	velocity -= multiplier2 * deltaTimeS;
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

void Car::computeVelocityAccelerationMultipliersMultiplier()
{
	// changeDirectionXTime *= interpolate((float)specificId, 0.0f, N_CARS - 1.0f, 1.0f, 2.0f);

	velocityMultiplier = interpolate((float)specificId, 0.0f, N_CARS - 1.0f, 1.0f, 0.7f);
	accelerationMultiplier = interpolate((float)specificId, 0.0f, N_CARS - 1.0f, 1.0f, 0.8f);
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

	list<const GameObject*> colliding;
	const GameObject* mainCollider = nullptr;
	
	if(getModuleWorld()->getGameEngine()->getModuleCollision()->getColliding(this, colliding))
	{
		for(list<const GameObject*>::const_iterator it = colliding.begin(); it != colliding.end(); ++it)
		{
			const GameObject* currentCollider = *it;

			// float z0 = mod0L(position.z + collider.b.d, getModuleWorld()->getRoad()->getLength());
			// float z1 = mod0L(currentCollider->g->getPosition()->z + currentCollider->b.d, getModuleWorld()->getRoad()->getLength());

			float z0 = position.z + box.d; // mod0L(position.z + collider.b.d, getModuleWorld()->getRoad()->getLength());
			float z1 = currentCollider->getPosition()->z + currentCollider->getBox()->d; // mod0L(currentCollider->g->getPosition()->z + currentCollider->b.d, getModuleWorld()->getRoad()->getLength());

			if(z1 < z0) continue;
			// if(currentCollider->g->getPosition()->z < position.z) continue;

			if(!mainCollider) mainCollider = currentCollider;
			else
			{
				switch(mainCollider->getType())
				{
					case GameObjectType::STATIC:
					case GameObjectType::ANIMATED:
						if(currentCollider->getType() == GameObjectType::STATIC || currentCollider->getType() == GameObjectType::ANIMATED)
							if(currentCollider->getBox()->w > mainCollider->getBox()->w)
								mainCollider = currentCollider;

						break;
					case GameObjectType::CAR:
					case GameObjectType::PLAYER:
						if(currentCollider->getType() == GameObjectType::STATIC || currentCollider->getType() == GameObjectType::ANIMATED)
							mainCollider = currentCollider;
				}
			}
		}
	}

	if(mainCollider)
	{
		if(getType() == GameObjectType::PLAYER || mainCollider->getType() == GameObjectType::PLAYER)
			getModuleWorld()->playFxCollision();

		velocity *= 0.5f;
		position.z -= /* getVelocityPercent() * */ SEGMENT_LENGTH; limitZ();

		globalZ -= SEGMENT_LENGTH;

		if(mainCollider->getType() == GameObjectType::STATIC || mainCollider->getType() == GameObjectType::ANIMATED)
		{
			float maxW = fmaxf(mainCollider->getBox()->w, box.w);

			if(mainCollider->getPosition()->x >= ROAD_MAX_X)
				position.x = ROAD_MAX_X - maxW;
			else if(mainCollider->getPosition()->x <= ROAD_MIN_X)
				position.x = ROAD_MIN_X + maxW;
			else if(mainCollider->getPosition()->x >= position.x)
				position.x -= mainCollider->getBox()->w;
			else
				position.x += mainCollider->getBox()->w;
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