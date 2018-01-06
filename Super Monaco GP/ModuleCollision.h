#ifndef _MODULE_COLLISION_
#define _MODULE_COLLISION_

#include <list>
#include "Module.h"
#include "Globals.h"

#define COLLISION_DISTANCE_CHECK (20.0f * SEGMENT_LENGTH)

class GameObject;

class ModuleCollision : public Module
{
	public:

		ModuleCollision(GameEngine* gameEngine);

		virtual ~ModuleCollision();

		bool getColliding(const GameObject* gameObject, std::list<const GameObject*>& colliding) const;

	private:

		bool collide(const GameObject* gameObject0, const GameObject* gameObject1) const;
};

#endif