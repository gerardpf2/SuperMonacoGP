#include "Car.h"

#include "Road.h"
#include "Utils.h"
#include "Segment.h"

Car::Car(const WorldPosition& position, const Road* road, Texture* texture):
	GameObject(position, road, texture)
{ }

Car::~Car()
{ }

void Car::update(float deltaTimeS)
{
	updateDirections(deltaTimeS);

	Segment* segment = road->getSegmentAtZ(position.z);

	position.z += velocity * deltaTimeS;
	
	limitZ();

	Segment* newSegment = road->getSegmentAtZ(position.z);

	float speedPercent = velocity / maxVelocity;
	float dX = 0.5f * ROAD_WIDTH * speedPercent * deltaTimeS;

	position.x += directionX * dX;
	position.x += 4.0f * ROAD_WIDTH * dX * speedPercent * newSegment->getCurve();

	position.y = interpolate(position.z, newSegment->getZNear(), newSegment->getZFar(), newSegment->getYNear(), newSegment->getYFar());

	velocity += directionZ * acceleration * deltaTimeS;
	velocity -= (1.0f - directionZ) * deacceleration * deltaTimeS;

	velocity = fminf(fmaxf(velocity, 0.0f), maxVelocity);

	if(segment != newSegment)
	{
		segment->eraseGameObject(this);
		newSegment->addGameObject(this);
	}
}

void Car::updateDirections(float deltaTimeS)
{ }