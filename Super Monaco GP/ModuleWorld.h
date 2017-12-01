#ifndef _MODULE_WORLD_
#define _MODULE_WORLD_

#include <vector>
#include "Module.h"

class Road;
class Camera;
class Player;
class GameObject;

class ModuleWorld : public Module
{
	public:

		ModuleWorld(GameEngine* gameEngine);

		virtual ~ModuleWorld();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		void addGameObject(GameObject* gameObject, float xOffsetRoad = 0.0f);

	private:

		Road* road = nullptr;

		Camera* camera = nullptr;

		Player* player = nullptr;

		std::vector<GameObject*> gameObjects;
};

#endif