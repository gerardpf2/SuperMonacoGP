#include "ModuleWorld.h"

#include "Road.h"
#include "Utils.h"
#include "Colors.h"
#include "Player.h"
#include "Segment.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "CameraFree.h"
#include "CameraFollow.h"

ModuleWorld::ModuleWorld(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModuleWorld::~ModuleWorld()
{ }

bool ModuleWorld::setUp()
{
	road = new Road();

	// GameObjects ---

	gameObjects.reserve(3);

	addGameObject(new GameObject(Position3f{ 0.0f, 0.0f, 20.0f }, road));
	addGameObject(new GameObject(Position3f{ 0.0f, 0.0f, 35.0f }, road));
	addGameObject(new GameObject(Position3f{ 0.0f, 0.0f, 50.0f }, road));

	addGameObject(new GameObject(Position3f{ 0.0f, 0.0f, 80.0f }, road));
	addGameObject(new GameObject(Position3f{ 0.0f, 0.0f, 95.0f }, road));
	addGameObject(new GameObject(Position3f{ 0.0f, 0.0f, 110.0f }, road));

	/* gameObjects.push_back(new GameObject(Position3f{ 0.0f, 0.0f, 0.0f }, road));
	gameObjects.push_back(new GameObject(Position3f{ 0.0f, 0.0f, 5.0f }, road));
	gameObjects.push_back(new GameObject(Position3f{ 0.0f, 0.0f, 10.0f }, road)); */

	// --- GameObjects

	// camera = new CameraFollow(player->getPosition(), road);
	camera = new CameraFree(road, getGameEngine()->getModuleInput());

	for(unsigned int i = 0; i < (unsigned int)gameObjects.size(); ++i)
	{
		GameObject* gameObject = gameObjects[i];
		
		Segment* segment = road->getSegmentAtZ(gameObject->getPosition()->third);
		
		gameObject->elevate(linear(gameObject->getPosition()->third, segment->getZNear(), segment->getZFar(), segment->yNear, segment->yFar));
	}

	return true;
}

bool ModuleWorld::preUpdate(float deltaTimeS)
{
	return true;
}

bool ModuleWorld::update(float deltaTimeS)
{
	for(unsigned int i = 0; i < (unsigned int)gameObjects.size(); ++i)
		gameObjects[i]->update(deltaTimeS);

	camera->update(deltaTimeS);

	return true;
}

#include <iostream>

bool ModuleWorld::postUpdate(float deltaTimeS)
{
	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	road->render(camera, moduleRenderer);

	// GameObjects ---

	/* unsigned int index = 0;
	float zCamera = camera->getPosition()->third;

	// int indexNear = getGameObjectIndexAt(zCamera);
	// int indexFar = getGameObjectIndexAt(zCamera + DRAW_DISTANCE);

	int indexNear, indexFar;
	getGameObjectIndicesAt(zCamera, zCamera + DRAW_DISTANCE, indexNear, indexFar);

	if(indexNear != -1 && indexFar != -1)
	{
		while(indexFar >= indexNear)
		{
			cout << zCamera << " . . . " << getGameObject(indexFar)->getPosition()->third << endl;

			getGameObject(indexFar)->render(road->getLength(), camera, moduleRenderer);
			--indexFar;
		}
	} */

	/* float zBase = 0.0f;
	float zOffset = 0.0f;
	float zCamera = camera->getPosition()->third;

	unsigned int index = 0;

	for(unsigned int i = 0; i < (unsigned int)gameObjects.size(); ++i)
	{
		float z = gameObjects[i]->getPosition()->third;
		
		if(z >= zCamera)
		{
			index = i;
			zBase = z;
			break;
		}
	}

	for(unsigned int i = 0; i < (unsigned int)gameObjects.size(); ++i)
	{
		gameObjects[i]->render(zOffset, camera, moduleRenderer);
	} */

	// --- GameObjects

	return true;
}

void ModuleWorld::cleanUp()
{
	road->clear();

	delete road;
	road = nullptr;

	delete camera;
	camera = nullptr;

	for(int i = (int)gameObjects.size() - 1; i >= 0; --i)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;
	}

	gameObjects.clear();
}

/* GameObject* ModuleWorld::getGameObject(int index) const
{
	return gameObjects[modI0ToL(index, gameObjects.size())];
} */

/* void ModuleWorld::getGameObjectIndicesAt(float z0, float z1, int& index0, int& index1) const
{
	index0 = index1 = -1;

	z0 = modF0ToL(z0, road->getLength());
	z1 = modF0ToL(z1, road->getLength());

	for(int i = 0; i < (int)gameObjects.size(); ++i)
		if(gameObjects[i]->getPosition()->third >= z0)
		{
			index0 = i;
			break;
		}

	for(int i = 0; i < (int)gameObjects.size(); ++i)
		if(gameObjects[i]->getPosition()->third > z1)
		{
			index1 = i;
			break;
		}
} */

/* int ModuleWorld::getGameObjectIndexAt(float z) const
{
	z = modF0ToL(z, road->getLength());

	for(int i = 0; i < (int)gameObjects.size(); ++i)
		if(gameObjects[i]->getPosition()->third >= z)
			return i;

	return -1;
} */

void ModuleWorld::addGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
	road->getSegmentAtZ(gameObject->getPosition()->third)->gameObjects.push_back(gameObject);
}