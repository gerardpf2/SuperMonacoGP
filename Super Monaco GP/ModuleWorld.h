#ifndef _MODULE_WORLD_
#define _MODULE_WORLD_

#include <vector>
#include "Types.h"
#include "Module.h"

class Road;
class Camera;
class Player;
class Background;
class GameObject;

class ModuleWorld : public Module
{
	public:

		ModuleWorld(GameEngine* gameEngine);

		virtual ~ModuleWorld();

		const Road* getRoad() const;

		const Camera* getCamera() const;

		const Camera* getCameraMirror() const;

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

		Player* player = nullptr;

		std::vector<GameObject*> gameObjects;

		Background* background = nullptr;

		Background* backgroundMirror = nullptr;

		// Rendering layers

		uint layerRoad, layerRoadMirror;
};

#endif