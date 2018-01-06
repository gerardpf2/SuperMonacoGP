#include "ModuleCollision.h"

#include "Road.h"
#include "Utils.h"
#include "Segment.h"
#include "GameObject.h"
#include "ModuleWorld.h"

using namespace std;

ModuleCollision::ModuleCollision(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleCollision::~ModuleCollision()
{ }

bool ModuleCollision::getColliding(const GameObject* gameObject, list<const GameObject*>& colliding) const
{
	assert(gameObject);
	assert(gameObject->getPosition());
	assert(gameObject->getModuleWorld());

	colliding.clear();

	float z = gameObject->getPosition()->z;
	float z0 = z - COLLISION_DISTANCE_CHECK;
	float z1 = z + COLLISION_DISTANCE_CHECK;

	const Road* road = gameObject->getModuleWorld()->getRoad();
	
	assert(road);

	for(float zz = z0; zz < z1; zz += SEGMENT_LENGTH)
	{
		Segment* segment = road->getSegmentAtZ(zz);

		assert(segment);
		assert(segment->getGameObjects());

		for(const GameObject* segmentGameObject : *segment->getGameObjects())
			if(segmentGameObject != gameObject && collide(segmentGameObject, gameObject))
				colliding.push_back(segmentGameObject);
	}

	return !colliding.empty();
}

bool ModuleCollision::collide(const GameObject* gameObject0, const GameObject* gameObject1) const
{
	assert(gameObject0);
	assert(gameObject1);

	const Box* box0 = gameObject0->getBox();
	const Box* box1 = gameObject1->getBox();

	assert(box0);
	assert(box1);

	if(box0->d <= 0.0f || box1->d <= 0.0f) return false;

	assert(gameObject0->getModuleWorld());
	assert(gameObject0->getModuleWorld()->getRoad());

	float roadLength = gameObject0->getModuleWorld()->getRoad()->getLength();

	const WorldPosition* position0 = gameObject0->getPosition();
	const WorldPosition* position1 = gameObject1->getPosition();

	assert(position0);
	assert(position1);

	// When computing y00 and y10 (bottom y of collider0 and bottom y of collider1) use its base y - 5.0f (arbitrary but large value)
	// in order not to avoid their collision if they land on a hill

	float x00 = position0->x - box0->w / 2.0f;
	float y00 = position0->y - 5.0f;
	float z00 = position0->z;

	float x01 = x00 + box0->w;
	float y01 = position0->y + box0->h;

	float z01 = mod0L(z00 + box0->d, roadLength);

	float x10 = position1->x - box1->w / 2.0f;
	float y10 = position1->y - 5.0f;
	float z10 = position1->z;

	float x11 = x10 + box1->w;
	float y11 = position1->y + box1->h;
	float z11 = mod0L(z10 + box1->d, roadLength);

	bool noCollideZ;

	if(z00 < z01 && z10 < z11) noCollideZ = z00 > z11 || z01 < z10;
	else if(z00 >= z01 && z10 >= z11) noCollideZ = false;
	else noCollideZ = z00 > z11 && z01 < z10;

	return !((x00 > x11 || x01 < x10) || (y00 > y11 || y01 < y10) || noCollideZ);
}