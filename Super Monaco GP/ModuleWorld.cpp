#include "ModuleWorld.h"

#include "Road.h"
#include "Camera.h"
#include "GameEngine.h"

ModuleWorld::ModuleWorld(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModuleWorld::~ModuleWorld()
{ }

bool ModuleWorld::setUp()
{
	camera = new Camera();
	road = new Road(1000.0f);

	return true;
}

// Test ---

#include "ModuleInput.h"
#include "ModuleRenderer.h"

float x = 0.0f;
float z = 0.0f;

// --- Test

bool ModuleWorld::preUpdate()
{
	// Test ---

	ModuleInput* moduleInput = getGameEngine()->getModuleInput();

	if(moduleInput->getKeyPressed(SDL_SCANCODE_UP)) z += 0.1f;
	if(moduleInput->getKeyPressed(SDL_SCANCODE_DOWN)) z -= 0.1f;
	if(moduleInput->getKeyPressed(SDL_SCANCODE_LEFT)) x -= 0.1f;
	if(moduleInput->getKeyPressed(SDL_SCANCODE_RIGHT)) x += 0.1f;

	if(moduleInput->getKeyPressed(SDL_SCANCODE_W)) camera->moveY(0.1f);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_S)) camera->moveY(-0.1f);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_D)) camera->moveXZ(0.1f, 0.0f);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_A)) camera->moveXZ(-0.1f, 0.0f);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_E)) camera->moveXZ(0.0f, 0.1f);
	if(moduleInput->getKeyPressed(SDL_SCANCODE_Q)) camera->moveXZ(0.0f, -0.1f);

	// camera->follow(Position3f{ x, 0.0f, z });

	// --- Test

	return true;
}

bool ModuleWorld::update(float deltaTimeS)
{
	return true;
}

bool ModuleWorld::postUpdate()
{
	// Test ---

	Position2i p0, p1, p2, p3, p4, p5;

	camera->getPositionWorldToScreen(Position3f{ x, 0.0f, z }, p0);
	camera->getPositionWorldToScreen(Position3f{ x, 5.0f, z }, p1);

	camera->getPositionWorldToScreen(Position3f{ ROAD_MIN_X, 0.0f, camera->getPosition()->z + 0.1f }, p2);
	camera->getPositionWorldToScreen(Position3f{ ROAD_MIN_X, 0.0f, 1000.0f }, p3);

	camera->getPositionWorldToScreen(Position3f{ ROAD_MAX_X, 0.0f, camera->getPosition()->z + 0.1f }, p4);
	camera->getPositionWorldToScreen(Position3f{ ROAD_MAX_X, 0.0f, 1000.0f }, p5);

	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	moduleRenderer->renderLine(p2, p3, WHITE);
	moduleRenderer->renderLine(p4, p5, WHITE);

	moduleRenderer->renderLine(Position2i{ 0, 240 }, Position2i{ 640, 240 }, WHITE);
	moduleRenderer->renderLine(Position2i{ 320, 0 }, Position2i{ 320, 480 }, WHITE);

	moduleRenderer->renderRect(SDL_Rect{ p0.x - 5, p0.y - 5, 10, 10 }, RED);
	moduleRenderer->renderRect(SDL_Rect{ p1.x - 5, p1.y - 5, 10, 10 }, BLUE);

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
}