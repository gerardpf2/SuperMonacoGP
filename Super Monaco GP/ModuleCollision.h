#ifndef _MODULE_COLLISION_
#define _MODULE_COLLISION_

#include <map>
#include <list>
#include <vector>
#include "Module.h"
#include "Globals.h"

#define COLLISION_DISTANCE_CHECK (20 * SEGMENT_LENGTH)

// struct Collider;

class GameObject;

class ModuleCollision : public Module
{
	public:

		ModuleCollision(GameEngine* gameEngine);

		virtual ~ModuleCollision();

		bool getColliding(const GameObject* gameObject, std::list<const GameObject*>& colliding) const;

		// void addCollider(const Collider* collider);

		// void removeColliders();

		// virtual bool update(float deltaTimeS) override;

	private:

		bool collide(const GameObject* gameObject0, const GameObject* gameObject1) const;

	// private:

		// std::vector<const Collider*> colliders;

		// std::map<const Collider*, std::list<const Collider*>> collisions;
};

#endif