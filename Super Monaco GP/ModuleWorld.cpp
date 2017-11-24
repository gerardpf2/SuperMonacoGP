#include "ModuleWorld.h"

#include "Road.h"
#include "Utils.h"
#include "Colors.h"
#include "Player.h"
#include "Segment.h"
#include <SDL_rect.h>
#include "GameObject.h"
#include "GameEngine.h"
#include "CameraFree.h"
#include "ModuleJson.h"
#include "CameraFollow.h"
#include "ModuleTexture.h"

ModuleWorld::ModuleWorld(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModuleWorld::~ModuleWorld()
{ }

bool ModuleWorld::setUp()
{
	Document roadDocument;
	getGameEngine()->getModuleJson()->read("Resources/Roads/Test2.json", roadDocument);

	road = new Road(roadDocument);

	// GameObjects ---

	gameObjects.reserve(7);

	// Test texture
	Texture* texture = new Texture{ getGameEngine()->getModuleTexture()->getTexture(TextureType::TEST), SDL_Rect{ 184, 11, 80, 45 } };

	addGameObject(player = new Player(WorldPosition{ 0.0f, 0.0f, 0.0f }, road, getGameEngine()->getModuleInput(), texture));

	addGameObject(new Car(WorldPosition{ 0.0f, 0.0f, 10.0f }, road, texture));

	addGameObject(new GameObject(WorldPosition{ -2.0f, 0.0f, 4.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 2.0f, 0.0f, 6.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 8.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 15.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 35.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 45.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 65.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 75.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 95.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 105.0f }, road, texture));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 125.0f }, road, texture));

	// --- GameObjects

	camera = new CameraFollow(player->getPosition(), road);
	// camera = new CameraFree(road, getGameEngine()->getModuleInput());

	for(uint i = 0; i < (uint)gameObjects.size(); ++i)
	{
		GameObject* gameObject = gameObjects[i];
		
		Segment* segment = road->getSegmentAtZ(gameObject->getPosition()->z);
		
		gameObject->elevate(interpolate(gameObject->getPosition()->z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar()));
	}

	return true;
}

#include "ModuleRenderer.h"

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
	road->clear();

	delete road;
	road = nullptr;

	delete camera;
	camera = nullptr;

	player = nullptr;

	for(int i = (int)gameObjects.size() - 1; i >= 0; --i)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;
	}

	gameObjects.clear();
}

void ModuleWorld::addGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
	road->getSegmentAtZ(gameObject->getPosition()->z)->addGameObject(gameObject);
}