#ifndef _MODULE_WORLD_
#define _MODULE_WORLD_

#include <vector>
#include "Module.h"

using namespace std;

class Road;
class Camera;
class GameObject;

class ModuleWorld : public Module
{
	public:

		ModuleWorld(GameEngine* gameEngine, bool active = true);

		virtual ~ModuleWorld();

		virtual bool setUp() override;

		virtual bool preUpdate(float deltaTimeS) override;

		virtual bool update(float deltaTimeS) override;

		virtual bool postUpdate(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		Road* road = nullptr;

		Camera* camera = nullptr;

		vector<GameObject*> gameObjects;
};

#endif