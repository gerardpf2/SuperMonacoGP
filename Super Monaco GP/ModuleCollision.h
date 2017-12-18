#ifndef _MODULE_COLLISION_
#define _MODULE_COLLISION_

#include <map>
#include <list>
#include <vector>
#include "Module.h"

struct Collider;

class GameObject;

class ModuleCollision : public Module
{
	public:

		ModuleCollision(GameEngine* gameEngine);

		virtual ~ModuleCollision();

		bool getColliding(const Collider* collider, const std::list<const Collider*>*& colliding) const;

		void addCollider(const Collider* collider);

		virtual bool update(float deltaTimeS) override;

	private:

		bool collide(const Collider* collider0, const Collider* collider1) const;

	private:

		std::vector<const Collider*> colliders;

		std::map<const Collider*, std::list<const Collider*>> collisions;
};

#endif