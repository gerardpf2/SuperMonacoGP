#include "Car.h"

#include "Road.h"
#include "Utils.h"
#include "Camera.h"
#include "Segment.h"
#include "Animation.h"
#include "GameEngine.h"
#include "ModuleWorld.h"
#include "ModuleCollision.h"
#include "AnimationContainer.h"

using namespace std;

Car::Car(AnimationContainer* animationContainer) :
	Animated(animationContainer)
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

	// Both velocity and acceleration multipliers depend on its specificId

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
	ModuleWorld* moduleWorld = getModuleWorld();

	assert(moduleWorld);

	Animated::update(deltaTimeS);

	// Compute all the current movement related variables

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
			moduleWorld->playFxOutRoad();
	}

	maxVelocity *= velocityMultiplier;
	acceleration *= accelerationMultiplier;

	// Compute the direction x and z

	updateDirection(deltaTimeS);

	const Road* road = moduleWorld->getRoad();

	assert(road);

	Segment* segment = road->getSegmentAtZ(position.z);

	assert(segment);

	// Update the z coordinate

	position.z += velocity * deltaTimeS;
	
	globalZ += velocity * deltaTimeS;

	// Check and resolve collisions

	checkCollision();

	limitZ();

	// Update the x coordinate

	float velocityPercent = getVelocityPercent();

	float dX = 0.75f * ROAD_WIDTH * velocityPercent * deltaTimeS;

	position.x += direction.x * dX;

	Segment* newSegment = road->getSegmentAtZ(position.z);

	assert(newSegment);

	updateOffsetX(dX, velocityPercent, newSegment->getCurve());

	// Update the y coordinate

	position.y = interpolate(position.z, newSegment->getZNear(), newSegment->getZFar(), newSegment->getYNear(), newSegment->getYFar());

	// Update the acceleration
	// The fastest the car goes, the more difficult to reach higher accelerations

	acceleration *= (1.0f - powf(velocityPercent, 2.0f));

	// Update the velocity

	velocity -= deaccelerationFriction * deltaTimeS;

	if(movementEnabled)
	{
		if(direction.z == 1.0f) velocity += acceleration * deltaTimeS;
		else if(direction.z == -1.0f) velocity -= deacceleration * deltaTimeS;

		currentLapTime += deltaTimeS;
	}

	if(velocity > CAR_DEACCELERATION_FRICTION_LIMIT_VELOCITY)
		velocity -= deaccelerationFrictionExtra * deltaTimeS;

	updateVelocityCurve(deltaTimeS);

	velocity = clamp(velocity, 0.0f, maxVelocity);

	position.x = clamp(position.x, CAR_MIN_X, CAR_MAX_X);

	// The car has been moved
	// Check if it lies on a different segment

	if(segment != newSegment)
	{
		segment->eraseGameObject(this);
		newSegment->addGameObject(this);
	}

	// Check if the car has completed a lap

	if(lapCompleted())
	{
		if(!ignoreCurrentLap)
		{
			registerLapTime();

			currentLapTimeStored = currentLapTime;
			currentLapTime = 0.0f;
			++currentLap;
		}
		else ignoreCurrentLap = false;
	}
}

void Car::updateDirection(float deltaTimeS)
{
	// AI Collision avoidance
	// Check for imminent collisions and try to avoid them

	direction.x = 0.0f; // Default direction x
	direction.z = 1.0f; // Default direction z

	float w = box.w;
	float x = position.x;
	float collisionLeftX = x - 0.5f * w;
	float collisionRightX = x + 0.5f * w;

	assert(getModuleWorld());
	assert(getModuleWorld()->getRoad());

	list<const GameObject*> obstaclesFront;
	getModuleWorld()->getRoad()->findGameObjectsFront(position.z, DIRECTION_Z_FAR, obstaclesFront);

	for(const GameObject* obstacle : obstaclesFront)
	{
		assert(obstacle);

		if(obstacle == this) continue;

		if(obstacle->getType() == GameObjectType::CAR || obstacle->getType() == GameObjectType::PLAYER)
			if(((Car*)obstacle)->getVelocityPercent() > getVelocityPercent()) continue;

		assert(obstacle->getBox());
		assert(obstacle->getPosition());

		float obstacleW = obstacle->getBox()->w;
		float obstacleX = obstacle->getPosition()->x;
		float obstacleCollisionLeftX = obstacleX - 0.75f * obstacleW;
		float obstacleCollisionRightX = obstacleX + 0.75f * obstacleW;

		bool mayCollide = !(obstacleCollisionLeftX > collisionRightX || obstacleCollisionRightX < collisionLeftX);

		if(mayCollide)
		{
			// Collision can happen
			// Check if it is better to move to the left or to the right

			float remainingSpaceObstacleLeftX = fabsf(ROAD_MIN_X - obstacleCollisionLeftX);
			float remainingSpaceObstacleRightX = fabsf(ROAD_MAX_X - obstacleCollisionRightX);

			float diffZ = fabsf(obstacle->getPosition()->z - position.z);
			
			float directionLeftX = interpolate(diffZ, 0.0f, DIRECTION_Z_FAR, -1.0f, 0.0f);
			float directionRightX = -directionLeftX;

			if(x <= obstacleX && remainingSpaceObstacleLeftX > w) direction.x = directionLeftX;
			else if(x > obstacleX && remainingSpaceObstacleRightX > w) direction.x = directionRightX;
			else direction.x = (remainingSpaceObstacleLeftX >= remainingSpaceObstacleRightX) ? directionLeftX : directionRightX;
			
			// If the collider is really close, break

			if(diffZ < DIRECTION_Z_CLOSE) direction.z = -1.0;

			// When building the possible collisions list, the closest ones go first
			// If it has been found a collision to avoid, that is the most imminent one
			// and there is no need to keep checking for more collisions

			break;
		}
	}

	direction.x = clamp(direction.x, -1.0f, 1.0f);
	direction.z = clamp(direction.z, -1.0f, 1.0f);
}

void Car::updateVelocityCurve(float deltaTimeS)
{
	assert(getModuleWorld());
	assert(getModuleWorld()->getRoad());

	Segment* segment = getModuleWorld()->getRoad()->getSegmentAtZ(position.z);
	
	assert(segment);

	// Reduce the velocity because of the curve

	float multiplier0 = interpolate(fabsf(segment->getCurve()), 0.0f, 0.02f, 0.0f, 10.0f);
	velocity -= multiplier0 * deltaTimeS;

	// Reduce the velocity because of the direction x

	float multiplier1 = getVelocityPercent() * interpolate(fabsf(direction.x), 0.0f, 1.0f, 0.0f, 2.0f);
	velocity -= multiplier1 * deltaTimeS;
}

void Car::updateCurrentAnimation(float deltaTimeS) const
{
	assert(animationContainer);

	Animation* currentAnimation = animationContainer->getCurrentAnimation();

	assert(currentAnimation);

	currentAnimation->setTimeMultiplier(getVelocityPercent());

	Animated::updateCurrentAnimation(deltaTimeS);

	/*
	
	AnimationId 0 -> Center
	AnimationId 1 -> Camera right side
	AnimationId 2 -> Camera left side
	AnimationId 3 -> Camera right side +
	AnimationId 4 -> Camera left side +
	
	*/

	assert(getModuleWorld());

	const Road* road = getModuleWorld()->getRoad();
	const Camera* camera = getModuleWorld()->getCamera();

	assert(road);
	assert(camera);
	assert(camera->getPosition());

	Animation* nextAnimation = currentAnimation;

	float x = position.x;
	float cameraX = camera->getPosition()->x;

	Segment* carSegment = road->getSegmentAtZ(position.z);
	Segment* cameraSegment = road->getSegmentAtZ(camera->getBasePositionZ());

	assert(carSegment);
	assert(cameraSegment);

	x -= carSegment->getXOffsetNear();
	cameraX -= cameraSegment->getXOffsetNear();

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
		assert(nextAnimation);

		nextAnimation->synchronize(*currentAnimation);
		animationContainer->setCurrentAnimation(nextAnimation->getId());
	}
}

void Car::updateOffsetX(float dX, float velocityPercent, float curve)
{ }

const Texture* Car::getCurrentTexture(bool mirror) const
{
	assert(animationContainer);

	Animation* currentAnimation = animationContainer->getCurrentAnimation();

	assert(currentAnimation);

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
	velocityMultiplier = interpolate((float)specificId, 0.0f, N_CARS - 1.0f, MAX_VELOCITY_MULTIPLIER, MIN_VELOCITY_MULTIPLIER);
	accelerationMultiplier = interpolate((float)specificId, 0.0f, N_CARS - 1.0f, MAX_ACCELERATION_MULTIPLIER, MIN_ACCELERATION_MULTIPLIER);
}

void Car::checkCollision()
{
	assert(getModuleWorld());
	assert(getModuleWorld()->getGameEngine());
	assert(getModuleWorld()->getGameEngine()->getModuleCollision());

	// Find all the game objects that are colliding to this car at this moment
	// Decide which one of these colliders is the most relevant
	// Priority: Static = Animated > Car = Player

	list<const GameObject*> colliding;
	const GameObject* mainCollider = nullptr;
	
	if(getModuleWorld()->getGameEngine()->getModuleCollision()->getColliding(this, colliding))
	{
		// Some colliders have been found
		// Find the most relevant one

		for(const GameObject* currentCollider : colliding)
		{
			assert(currentCollider);
			assert(currentCollider->getBox());
			assert(currentCollider->getPosition());

			float z0 = position.z + box.d;
			float z1 = currentCollider->getPosition()->z + currentCollider->getBox()->d;

			// Ignore all the colliders that are behind this car
			// They will resolve their collision with this one

			if(z1 < z0) continue;

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
		// The most relevant collider has been found

		if(getType() == GameObjectType::PLAYER || mainCollider->getType() == GameObjectType::PLAYER)
			getModuleWorld()->playFxCollision();

		velocity *= 0.5f;
		position.z -= SEGMENT_LENGTH; limitZ();

		globalZ -= SEGMENT_LENGTH;

		if(mainCollider->getType() == GameObjectType::STATIC || mainCollider->getType() == GameObjectType::ANIMATED)
		{
			assert(mainCollider->getBox());
			assert(mainCollider->getPosition());

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

bool Car::lapCompleted()
{
	assert(getModuleWorld());
	assert(getModuleWorld()->getRoad());

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
}

void Car::registerLapTime() const
{
	assert(getModuleWorld());

	getModuleWorld()->registerLapTime(this);
}