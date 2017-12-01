#include "ModuleWorld.h"

#include "Road.h"
#include <vector>
#include "Utils.h"
#include "Player.h"
#include "Segment.h"
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

	road->load("Resources/Configurations/Roads/Test1.json", getGameEngine()->getModuleJson());

	// GameObjects ---

	gameObjects.reserve(1);

	// getGameEngine()->getModuleAnimation()->load("Resources/Configurations/Animations/AnimationsTest.json");

	// AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(0, 0);
	// AnimationContainer* animationContainer2 = getGameEngine()->getModuleAnimation()->getAnimationContainer(0, 0);
	// AnimationContainer* animationContainer3 = getGameEngine()->getModuleAnimation()->getAnimationContainer(0, 0);

	// player = new Player(0, animationContainer, getGameEngine()->getModuleInput());
	player = (Player*)getGameEngine()->getModuleGameObject()->getGameObject(0);
	player->setRoad(road);
	player->setPosition(WorldPosition{ 0.0f, 0.0f, 0.0f });

	addGameObject(player);

	Car* car = (Car*)getGameEngine()->getModuleGameObject()->getGameObject(1);
	car->setRoad(road);
	car->setPosition(WorldPosition{ 7.5f, 0.0f, 10.0f });

	addGameObject(car);

	/* Car* car = new Car(1, animationContainer2);
	car->setRoad(road);
	car->setPosition(WorldPosition{ 0.0f, 0.0f, 10.0f }); */

	/* GameObject* gameObject0 = new GameObject(animationContainer3);
	gameObject0->setRoad(road);
	gameObject0->setPosition(WorldPosition{ 0.0f, 0.0f, 20.0f });

	GameObject* gameObject1 = new GameObject(animationContainer3);
	gameObject1->setRoad(road);
	gameObject1->setPosition(WorldPosition{ 4.0f, 10.0f, 10.0f });

	GameObject* gameObject2 = new GameObject(animationContainer3);
	gameObject2->setRoad(road);
	gameObject2->setPosition(WorldPosition{ 0.0f, 10.0f, 10.0f });

	GameObject* gameObject3 = new GameObject(animationContainer3);
	gameObject3->setRoad(road);
	gameObject3->setPosition(WorldPosition{ -4.0f, 10.0f, 10.0f }); */

	// addGameObject(car);
	/* addGameObject(gameObject0, 1.0);
	addGameObject(gameObject1);
	addGameObject(gameObject2);
	addGameObject(gameObject3); */
	
	// --- GameObjects

	camera = new CameraFollow(road, player->getPosition());

	for(GameObject* gameObject : gameObjects)
		gameObject->elevate();

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

void ModuleWorld::addGameObject(GameObject* gameObject, float xOffsetRoad)
{
	gameObject->moveX(xOffsetRoad * (ROAD_WIDTH / 2.0f));

	gameObjects.push_back(gameObject);
	road->getSegmentAtZ(gameObject->getPosition()->z)->addGameObject(gameObject);
}