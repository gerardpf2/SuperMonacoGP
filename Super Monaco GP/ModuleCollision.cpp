#include "ModuleCollision.h"

// REVISAR

#include "Road.h"
#include "Utils.h"
#include "Segment.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "ModuleWorld.h"

using namespace std;

ModuleCollision::ModuleCollision(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleCollision::~ModuleCollision()
{ }

bool ModuleCollision::getColliding(const GameObject* gameObject, list<const GameObject*>& colliding) const
{
	colliding.clear();

	float z = gameObject->getPosition()->z;
	float z0 = z - COLLISION_DISTANCE_CHECK;
	float z1 = z + COLLISION_DISTANCE_CHECK;

	const Road* road = gameObject->getModuleWorld()->getRoad();
	
	for(float zz = z0; zz < z1; zz += SEGMENT_LENGTH)
	{
		Segment* segment = road->getSegmentAtZ(zz);

		for(const GameObject* segmentGameObject : *segment->getGameObjects())
			if(segmentGameObject != gameObject && collide(segmentGameObject, gameObject))
				colliding.push_back(segmentGameObject);
	}

	return !colliding.empty();

	/* colliding = nullptr;

	map<const Collider*, list<const Collider*>>::const_iterator it = collisions.find(collider);
	if(it != collisions.end()) colliding = &it->second;

	return colliding && colliding->size() > 0; */
}

/* void ModuleCollision::addCollider(const Collider* collider)
{
	if(collider->b.d > 0.0f) colliders.push_back(collider);
} */

/* void ModuleCollision::removeColliders()
{
	// colliders.clear();
} */

// #include <iostream>

/* bool ModuleCollision::update(float deltaTimeS)
{
	collisions.clear();

	for(uint i = 0; i < (uint)colliders.size(); ++i)
	{
		const Collider* collider0 = colliders[i];

		for(uint j = i + 1; j < (uint)colliders.size(); ++j)
		{
			const Collider* collider1 = colliders[j];

			if(collide(collider0, collider1))
			{
				collisions[collider0].push_back(collider1);
				collisions[collider1].push_back(collider0);
			}
		}
	}

	return true;
} */

bool ModuleCollision::collide(const GameObject* gameObject0, const GameObject* gameObject1) const
{
	/*
	
	When computing y00 and y10 (bottom y of collider0 and collider1) use its base y - 5.0f (arbitrary but large value)
	in order not to avoid their collision if they land on a hill
	
	*/

	const Box* box0 = gameObject0->getBox();
	const Box* box1 = gameObject1->getBox();

	if(box0->d <= 0.0f || box1->d <= 0.0f) return false;

	float x00 = gameObject0->getPosition()->x - box0->w / 2.0f;
	float y00 = gameObject0->getPosition()->y - 5.0f; // - collider0->b.h / 2.0f;
	float z00 = gameObject0->getPosition()->z; // - collider0->b.d / 2.0f;

	float x01 = x00 + box0->w;
	float y01 = gameObject0->getPosition()->y + box0->h;

	float z01 = mod0L(z00 + box0->d, gameObject0->getModuleWorld()->getRoad()->getLength());
	// float z01 = mod0L(z00 + collider0->b.d, getGameEngine()->getModuleWorld()->getRoad()->getLength());

	float x10 = gameObject1->getPosition()->x - box1->w / 2.0f;
	float y10 = gameObject1->getPosition()->y - 5.0f; // - collider1->b.h / 2.0f;
	float z10 = gameObject1->getPosition()->z; // - collider1->b.d / 2.0f;

	float x11 = x10 + box1->w;
	float y11 = gameObject1->getPosition()->y + box1->h;
	float z11 = mod0L(z10 + box1->d, gameObject1->getModuleWorld()->getRoad()->getLength());
	// float z11 = mod0L(z10 + collider1->b.d, getGameEngine()->getModuleWorld()->getRoad()->getLength());

	bool noCollideZ;

	if(z00 < z01 && z10 < z11) noCollideZ = z00 > z11 || z01 < z10;
	else if(z00 >= z01 && z10 >= z11) noCollideZ = false;
	else noCollideZ = z00 > z11 && z01 < z10;

	/* bool noCollideZ;

	if(z00 < z01)
	{
		if(z10 < z11)
		{
			noCollideZ = z00 > z11 || z01 < z10;
		}
		else
		{
			noCollideZ = z00 > z11 && z01 < z10;
		}
	}
	else
	{
		if(z10 < z11)
		{
			noCollideZ = z00 > z11 && z01 < z10;
		}
		else
		{
			noCollideZ = false;
		}
	} */

	return !((x00 > x11 || x01 < x10) || (y00 > y11 || y01 < y10) || noCollideZ);
	// return !((x00 > x11 || x01 < x10) || (y00 > y11 || y01 < y10) || ((z00 > z11 && z01 > z00) || (z01 < z10 && z11 > z10)));
}