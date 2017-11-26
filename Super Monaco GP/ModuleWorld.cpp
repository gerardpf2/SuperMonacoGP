#include "ModuleWorld.h"

#include "Road.h"
#include <vector>
#include "Utils.h"
#include "Colors.h"
#include "Player.h"
#include "Segment.h"
#include <SDL_rect.h>
#include "Animation.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "CameraFree.h"
#include "ModuleJson.h"
#include "CameraFollow.h"
#include "ModuleAnimation.h"
#include "AnimationGroup.h"

using namespace std;
using namespace rapidjson;

ModuleWorld::ModuleWorld(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleWorld::~ModuleWorld()
{ }

bool ModuleWorld::setUp()
{
	Document roadDocument;
	getGameEngine()->getModuleJson()->read("Resources/Roads/Test1.json", roadDocument);

	road = new Road(roadDocument);

	// GameObjects ---

	gameObjects.reserve(7);

	AnimationGroup* animationGroup = getGameEngine()->getModuleAnimation()->getAnimationGroupCopy(AnimationGroupType::PLAYER);
	AnimationGroup* animationGroup2 = getGameEngine()->getModuleAnimation()->getAnimationGroupCopy(AnimationGroupType::PLAYER);
	AnimationGroup* animationGroup3 = getGameEngine()->getModuleAnimation()->getAnimationGroupCopy(AnimationGroupType::PLAYER);

	addGameObject(player = new Player(WorldPosition{ 0.0f, 0.0f, 0.0f }, animationGroup, road, getGameEngine()->getModuleInput()));
	
	addGameObject(new Car(WorldPosition{ 0.0f, 0.0f, 10.0f }, animationGroup2, road));
	
	addGameObject(new GameObject(WorldPosition{ 5.0f, 0.0f, 5.0f }, animationGroup3, road));

	// --- GameObjects

	camera = new CameraFollow(road, player->getPosition());
	// camera = new CameraFree(road, getGameEngine()->getModuleInput());

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
	if(road)
	{
		road->clear();

		delete road;
		road = nullptr;
	}

	if(camera)
	{
		delete camera;
		camera = nullptr;
	}

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