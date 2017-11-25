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
#include "ModuleTexture.h"

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
	getGameEngine()->getModuleJson()->read("Resources/Roads/Test2.json", roadDocument);

	road = new Road(roadDocument);

	// GameObjects ---

	gameObjects.reserve(7);

	// Test texture
	Texture* texture = new Texture{ getGameEngine()->getModuleTexture()->getTexture(TextureType::TEST), SDL_Rect{ 184, 11, 80, 45 } };
	Texture* texture2 = new Texture{ getGameEngine()->getModuleTexture()->getTexture(TextureType::TEST), SDL_Rect{ 96, 67, 80, 45 } };

	// Test animation
	vector<const Texture*>* frames = new vector<const Texture*>();
	frames->reserve(2);
	frames->push_back(texture);
	frames->push_back(texture2);

	Animation* animation = new Animation(frames, 1.0f);

	vector<Animation*>* animations = new vector<Animation*>();
	animations->reserve(1);
	animations->push_back(animation);

	// addGameObject(player = new Player(WorldPosition{ 0.0f, 0.0f, 0.0f }, texture, road, getGameEngine()->getModuleInput()));
	addGameObject(player = new Player(WorldPosition{ 0.0f, 0.0f, 0.0f }, animations, road, getGameEngine()->getModuleInput()));
	
	/* addGameObject(new Car(WorldPosition{ 0.0f, 0.0f, 10.0f }, texture, road));

	addGameObject(new GameObject(WorldPosition{ -2.0f, 0.0f, 4.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 2.0f, 0.0f, 6.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 8.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 15.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 35.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 45.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 65.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 75.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 95.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 105.0f }, texture, road));
	addGameObject(new GameObject(WorldPosition{ 0.0f, 0.0f, 125.0f }, texture, road)); */

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