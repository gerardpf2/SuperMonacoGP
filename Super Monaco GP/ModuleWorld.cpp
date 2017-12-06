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
#include "ModuleRenderer.h"
#include "ModuleAnimation.h"
#include "ModuleGameObject.h"
#include "AnimationContainer.h"

using namespace std;

ModuleWorld::ModuleWorld(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleWorld::~ModuleWorld()
{ }

uint layerRoad = -1;
uint layerRoadMirror = -1;

SDL_Rect viewportRoadMirror{ WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };

bool ModuleWorld::setUp()
{
	road = new Road();

	road->load("Resources/Configurations/Roads/Test0.json", getGameEngine()->getModuleJson());

	// GameObjects ---

	const vector<RoadGameObjectDefinition*>* gameObjectDefinitions = road->getGameObjectDefinitions();

	gameObjects.reserve(gameObjectDefinitions->size() + 2);

	// Player

	player = addGameObject(0, WorldPosition{ 12.5f, 0.0f, 0.0f });

	// Cars

	addGameObject(1, WorldPosition{ 7.5f, 0.0f, 10.0f });

	// Environment

	for(uint i = 0; i < (uint)gameObjectDefinitions->size(); ++i)
	{
		RoadGameObjectDefinition* gameObjectDefinition = (*gameObjectDefinitions)[i];
		addGameObject(gameObjectDefinition->id, gameObjectDefinition->wp, gameObjectDefinition->offsetX);
	}
	
	// --- GameObjects

	camera = new CameraFollow(true, road, player->getPosition());
	cameraMirror = new CameraFollow(false, road, player->getPosition(), WorldPosition{ 0.0f, 0.0f, -CAMERA_Y });

	layerRoad = getGameEngine()->getModuleRenderer()->addLayer(nullptr);
	layerRoadMirror = getGameEngine()->getModuleRenderer()->addLayer(&viewportRoadMirror);

	return true;
}

bool ModuleWorld::update(float deltaTimeS)
{
	for(uint i = 0; i < (uint)gameObjects.size(); ++i)
		gameObjects[i]->update(deltaTimeS);

	camera->update(deltaTimeS);
	cameraMirror->update(deltaTimeS);

	/* getGameEngine()->getModuleRenderer()->setLayer(layerRoadMirror);
	road->render(camera, getGameEngine()->getModuleRenderer());

	getGameEngine()->getModuleRenderer()->setLayer(layerRoad);
	road->renderMirror(camera, getGameEngine()->getModuleRenderer()); */

	getGameEngine()->getModuleRenderer()->setLayer(layerRoadMirror);
	road->render(camera, getGameEngine()->getModuleRenderer());
	getGameEngine()->getModuleRenderer()->setLayer(layerRoad);
	road->renderMirror(cameraMirror, getGameEngine()->getModuleRenderer());

	// getGameEngine()->getModuleRenderer()->setLayer(layerRoadMirror);
	// road->renderMirror(camera, getGameEngine()->getModuleRenderer());

	return true;
}

void ModuleWorld::cleanUp()
{
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoad);
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoadMirror);

	getGameEngine()->getModuleAnimation()->unload(0); // ¿?

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

	if(cameraMirror)
	{
		delete cameraMirror;
		cameraMirror = nullptr;
	}

	player = nullptr;

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