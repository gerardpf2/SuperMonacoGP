#include "ModuleWorld.h"

#include "Road.h"
#include "GameEngine.h"
#include "GameObject.h"
#include "CameraFree.h"
#include "CameraFollow.h"

ModuleWorld::ModuleWorld(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModuleWorld::~ModuleWorld()
{ }

// Test ---

/* #include <iostream>
#include "ModuleInput.h"
#include "ModuleRenderer.h"

float x = 0.0f;
float z = 0.0f; */

// --- Test

bool ModuleWorld::setUp()
{
	road = new Road();
	// camera = new CameraFollow(&z, road);
	camera = new CameraFree(road, getGameEngine()->getModuleInput());

	// GameObjects ---

	gameObjects.reserve(1);

	gameObjects.push_back(new GameObject(50.0f, GREEN));

	// --- GameObjects

	return true;
}

bool ModuleWorld::preUpdate(float deltaTimeS)
{
	camera->update(deltaTimeS);

	// Test ---

	/* ModuleInput* moduleInput = getGameEngine()->getModuleInput();

	if(moduleInput->getKeyPressed(SDL_SCANCODE_UP)) z += 0.1f;
	if(moduleInput->getKeyPressed(SDL_SCANCODE_DOWN)) z -= 0.1f;
	if(moduleInput->getKeyPressed(SDL_SCANCODE_LEFT)) x -= 0.1f;
	if(moduleInput->getKeyPressed(SDL_SCANCODE_RIGHT)) x += 0.1f;

	z = modF0ToL(z, road->getLength()); */

	// --- Test

	return true;
}

bool ModuleWorld::update(float deltaTimeS)
{
	return true;
}

bool ModuleWorld::postUpdate(float deltaTimeS)
{
	road->render(camera, getGameEngine()->getModuleRenderer());

	/* float zStart = modF0ToL(camera->getBaseZ(), road->getLength());

	float zDrawn = 0.0f;
	unsigned int index = 0;

	for(unsigned int i = 0; i < (unsigned int)gameObjects.size(); ++i)
	{
		if(gameObjects[i]->getZ() >= zStart)
		{
			index = i;
			break;
		}
	}

	while(false && zDrawn < DRAW_DISTANCE) //
	{
		float z = gameObjects[index++ % gameObjects.size()]->getZ();

		// ...
	} */

	// Test ---

	/* Position2s p0, p1;

	camera->getPositionWorldToScreen(Position3f{ x, 0.0f, z }, p0);
	camera->getPositionWorldToScreen(Position3f{ x, 5.0f, z }, p1);

	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	moduleRenderer->renderRectangle(SDL_Rect{ p0.x - 5, p0.y - 5, 10, 10 }, RED, true);
	moduleRenderer->renderRectangle(SDL_Rect{ p1.x - 5, p1.y - 5, 10, 10 }, BLUE, true); */

	// --- Test

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