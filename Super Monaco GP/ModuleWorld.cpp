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
#include "Background.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "CameraFollow.h"
#include "ModuleTexture.h"
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

// Revisar
SDL_Rect textureRectRoadMirror{ 0, (int)((1.0f - 0.24f) * WINDOW_HEIGHT), WINDOW_WIDTH, (int)(0.24f * WINDOW_HEIGHT) };
SDL_Rect viewportRoadMirror{ (int)(0.15f * WINDOW_WIDTH), (int)(0.12f * WINDOW_HEIGHT), (int)(0.7f * WINDOW_WIDTH), (int)(0.17f * WINDOW_HEIGHT) };

bool ModuleWorld::setUp()
{
	road = new Road();

	road->load("Resources/Configurations/Roads/Test0.json", getGameEngine()->getModuleJson(), getGameEngine()->getModuleTexture());

	// GameObjects ---

	const vector<RoadGameObjectDefinition*>* gameObjectDefinitions = road->getGameObjectDefinitions();

	gameObjects.reserve(gameObjectDefinitions->size() + 2);

	// Player

	player = addGameObject(0, WorldPosition{ 0.0f, 0.0f, 0.0f });

	// Cars

	addGameObject(1, WorldPosition{ 7.0f, 0.0f, 10.0f });

	// Environment

	for(uint i = 0; i < (uint)gameObjectDefinitions->size(); ++i)
	{
		RoadGameObjectDefinition* gameObjectDefinition = (*gameObjectDefinitions)[i];
		addGameObject(gameObjectDefinition->id, gameObjectDefinition->wp, gameObjectDefinition->offsetX);
	}
	
	// --- GameObjects

	camera = new CameraFollow(true, road, player->getPosition());
	cameraMirror = new CameraFollow(false, road, player->getPosition(), 1.1f, 11.0f, WorldPosition{ 0.0f, 0.0f, -CAMERA_Y });

	// Background ---

	const RoadBackgroundDefinition* roadBackgroundDefinition = road->getRoadBackgroundDefinition();
	
	background = new Background(getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureSkyId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureGroundId), camera->getProjectionY1());
	backgroundMirror = new Background(getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureSkyId), getGameEngine()->getModuleTexture()->get(roadBackgroundDefinition->textureGroupId, roadBackgroundDefinition->textureGroundId), cameraMirror->getProjectionY1());

	// --- Background

	layerRoad = getGameEngine()->getModuleRenderer()->addLayer();
	layerRoadMirror = getGameEngine()->getModuleRenderer()->addLayer(&textureRectRoadMirror, &viewportRoadMirror);

	return true;
}

bool ModuleWorld::update(float deltaTimeS)
{
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
		paused = !paused;

	if(!paused)
	{
		for(uint i = 0; i < (uint)gameObjects.size(); ++i)
			gameObjects[i]->update(deltaTimeS);

		camera->update(deltaTimeS);
		cameraMirror->update(deltaTimeS);

		background->update((Player*)player, road, deltaTimeS);
		backgroundMirror->update((Player*)player, road, deltaTimeS);
	}

	getGameEngine()->getModuleRenderer()->setLayer(layerRoad);
	background->render(!camera->getForward(), getGameEngine()->getModuleRenderer());
	road->render(camera, getGameEngine()->getModuleRenderer());
	
	getGameEngine()->getModuleFont()->renderText("123 LALA 1'2''3", WindowPosition{ 100, 400 }); //

	getGameEngine()->getModuleRenderer()->setLayer(layerRoadMirror);
	backgroundMirror->render(!cameraMirror->getForward(), getGameEngine()->getModuleRenderer());
	road->render(cameraMirror, getGameEngine()->getModuleRenderer());

	return true;
}

void ModuleWorld::cleanUp()
{
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoad);
	getGameEngine()->getModuleRenderer()->removeLayer(layerRoadMirror);

	if(road)
	{
		road->unload(getGameEngine()->getModuleTexture());

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

	delete background;
	background = nullptr;

	delete backgroundMirror;
	backgroundMirror = nullptr;
}

GameObject* ModuleWorld::addGameObject(uint id, const WorldPosition& worldPosition, float xOffsetRoad)
{
	GameObject* gameObject = getGameEngine()->getModuleGameObject()->getGameObject(id);

	gameObject->setRoad(road);
	// gameObject->setPosition(worldPosition);

	// gameObject->moveX(xOffsetRoad * (ROAD_WIDTH / 2.0f));

	// Adjust game object's position
	WorldPosition wP = worldPosition;

	wP.x += xOffsetRoad * (ROAD_WIDTH / 2.0f);
	wP.z = road->getSegmentAtZ(wP.z)->getZNear() + SEGMENT_LENGTH / 2.0f;

	gameObject->setPosition(wP);
	gameObject->elevate();

	gameObjects.push_back(gameObject);
	road->getSegmentAtZ(gameObject->getPosition()->z)->addGameObject(gameObject);

	return gameObject;
}