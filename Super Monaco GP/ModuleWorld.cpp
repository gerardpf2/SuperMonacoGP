#include "ModuleWorld.h"

#include "Road.h"
#include <vector>
#include "Utils.h"
#include "Player.h"
#include "Static.h"
#include "Segment.h"
#include "Animated.h"
#include <SDL_rect.h>
#include "Animation.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "CameraFollow.h"
#include "ModuleAnimation.h"
#include "ModuleGameObject.h"
#include "AnimationContainer.h"

using namespace std;

ModuleWorld::ModuleWorld(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleWorld::~ModuleWorld()
{ }

bool ModuleWorld::setUp()
{
	road = new Road();

	road->load("Resources/Configurations/Roads/Test0.json", getGameEngine()->getModuleJson());

	// GameObjects ---

	const vector<RoadGameObjectDefinition*>* gameObjectDefinitions = road->getGameObjectDefinitions();

	gameObjects.reserve(gameObjectDefinitions->size() + 2);

	// Player

	player = addGameObject(0, WorldPosition{ 0.0f, 0.0f, 0.0f });

	// Cars

	addGameObject(1, WorldPosition{ 7.5f, 0.0f, 10.0f });

	// Environment

	for(uint i = 0; i < (uint)gameObjectDefinitions->size(); ++i)
	{
		RoadGameObjectDefinition* gameObjectDefinition = (*gameObjectDefinitions)[i];
		addGameObject(gameObjectDefinition->id, gameObjectDefinition->wp, gameObjectDefinition->offsetX);
	}
	
	// --- GameObjects

	camera = new CameraFollow(road, player->getPosition());

	return true;
}

bool ModuleWorld::update(float deltaTimeS)
{
	for(uint i = 0; i < (uint)gameObjects.size(); ++i)
		gameObjects[i]->update(deltaTimeS);

	camera->update(deltaTimeS);

	road->render(camera, getGameEngine()->getModuleRenderer());

	return true;
}

void ModuleWorld::cleanUp()
{
	getGameEngine()->getModuleAnimation()->unload(0);

	if(road)
	{
		road->unload();

		delete road;
		road = nullptr;
	}

	if(camera)
	{
		delete camera;
		camera = nullptr;
	}

	player = nullptr;

	/* for(int i = (int)gameObjects.size() - 1; i >= 0; --i)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;
	} */

	gameObjects.clear();
}

GameObject* ModuleWorld::addGameObject(uint id, const WorldPosition& worldPosition, float xOffsetRoad)
{
	GameObject* gameObject = getGameEngine()->getModuleGameObject()->getGameObject(id);

	gameObject->setRoad(road);
	gameObject->setPosition(worldPosition);
	
	gameObject->elevate();
	gameObject->moveX(xOffsetRoad * (ROAD_WIDTH / 2.0f));

	gameObjects.push_back(gameObject);
	road->getSegmentAtZ(gameObject->getPosition()->z)->addGameObject(gameObject);

	return gameObject;
}