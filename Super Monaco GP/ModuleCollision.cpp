#include "ModuleCollision.h"

#include "GameObject.h"

using namespace std;

ModuleCollision::ModuleCollision(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleCollision::~ModuleCollision()
{ }

bool ModuleCollision::getColliding(const Collider* collider, const std::list<const Collider*>*& colliding) const
{
	map<const Collider*, list<const Collider*>>::const_iterator it = collisions.find(collider);
	if(it != collisions.end()) colliding = &it->second;

	return colliding && colliding->size() > 0;
}

void ModuleCollision::addCollider(const Collider* collider)
{
	colliders.push_back(collider);
}

#include <iostream>

bool ModuleCollision::update(float deltaTimeS)
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

				if(collider0->g->getType() == GameObjectType::CAR)
					cout << "CAR: " << collider0->g->getPosition()->z << ", " << collider1->g->getPosition()->z << endl;
				else if(collider0->g->getType() == GameObjectType::PLAYER)
					cout << "PLAYER: " << collider0->g->getPosition()->z << ", " << collider1->g->getPosition()->z << endl;
			}
		}
	}

	return true;
}

bool ModuleCollision::collide(const Collider* collider0, const Collider* collider1) const
{
	float x00 = collider0->g->getPosition()->x - collider0->b.w / 2.0f;
	float y00 = collider0->g->getPosition()->y; // - collider0->b.h / 2.0f;
	float z00 = collider0->g->getPosition()->z - collider0->b.d / 2.0f;

	float x01 = x00 + collider0->b.w;
	float y01 = y00 + collider0->b.h;
	float z01 = z00 + collider0->b.d;

	float x10 = collider1->g->getPosition()->x - collider1->b.w / 2.0f;
	float y10 = collider1->g->getPosition()->y; // - collider1->b.h / 2.0f;
	float z10 = collider1->g->getPosition()->z - collider1->b.d / 2.0f;

	float x11 = x10 + collider1->b.w;
	float y11 = y10 + collider1->b.h;
	float z11 = z10 + collider1->b.d;

	return !((x00 > x11 || x01 < x10) || (y00 > y11 || y01 < y10) || (z00 > z11 || z01 < z10));
}