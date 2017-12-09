#ifndef _MODULE_WORLD_
#define _MODULE_WORLD_

#include <vector>
#include "Types.h"
#include "Module.h"

class Road;
class Camera;
class Background;
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

		GameObject* addGameObject(uint id, const WorldPosition& worldPosition, float xOffsetRoad = 0.0f);

	private:

		bool paused = false;

		Road* road = nullptr;

		Camera* camera = nullptr;

		Camera* cameraMirror = nullptr;

		GameObject* player = nullptr;

		std::vector<GameObject*> gameObjects;

		Background* background = nullptr;

		Background* backgroundMirror = nullptr;

		// Rendering layers

		uint layerRoad, layerRoadMirror;
};

#endif