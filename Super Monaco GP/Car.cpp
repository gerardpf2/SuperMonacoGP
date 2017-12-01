#include "Car.h"

#include "Road.h"
#include "Utils.h"
#include "Segment.h"
#include "Animation.h"
#include "AnimationContainer.h"

using namespace std;

Car::Car(uint id, const AnimationContainer* animationContainer) :
	GameObject(id, animationContainer)
{ }

Car::~Car()
{ }

GameObjectType Car::getType() const
{
	return GameObjectType::CAR;
}

float Car::getVelocity() const
{
	return velocity;
}

void Car::update(float deltaTimeS)
{
	GameObject::update(deltaTimeS);

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

	Segment* segment = getRoad()->getSegmentAtZ(position.z);

	position.z += velocity * deltaTimeS;
	
	limitZ();

	Segment* newSegment = getRoad()->getSegmentAtZ(position.z);

	float speedPercent = velocity / maxVelocity;

	animationContainer->getCurrentAnimation()->setTimeMultiplier(speedPercent);
	
	float dX = 0.5f * ROAD_WIDTH * speedPercent * deltaTimeS;

	position.x += direction.x * dX;
	position.x += 4.0f * ROAD_WIDTH * dX * speedPercent * newSegment->getCurve();

	position.x = clamp(position.x, CAR_MIN_X, CAR_MAX_X);

	position.y = interpolate(position.z, newSegment->getZNear(), newSegment->getZFar(), newSegment->getYNear(), newSegment->getYFar());

	acceleration *= (1.0f - powf(speedPercent, 2.0f));

	velocity -= deaccelerationFriction * deltaTimeS;

	if(direction.z == 1.0f) velocity += acceleration * deltaTimeS;
	else if(direction.z == -1.0f) velocity -= deacceleration * deltaTimeS;

	if(velocity > CAR_DEACCELERATION_FRICTION_LIMIT_VELOCITY)
		velocity -= deaccelerationFrictionExtra * deltaTimeS;

	velocity = clamp(velocity, 0.0f, maxVelocity);

	if(segment != newSegment)
	{
		segment->eraseGameObject(this);
		newSegment->addGameObject(this);
	}
}

void Car::updateDirection(float deltaTimeS)
{
	direction.x = 0.0f;
	direction.z = 1.0f;
}