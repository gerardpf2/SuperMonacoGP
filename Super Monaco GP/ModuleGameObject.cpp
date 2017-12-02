#include "ModuleGameObject.h"

#include "Car.h"
#include "Player.h"
#include "Static.h"
#include "Animated.h"
#include "ModuleJson.h"
#include "GameEngine.h"
#include "GameObject.h"
#include "ModuleTexture.h"
#include "ModuleAnimation.h"
#include "AnimationContainer.h"

using namespace std;
using namespace rapidjson;

ModuleGameObject::ModuleGameObject(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleGameObject::~ModuleGameObject()
{ }

GameObject* ModuleGameObject::getGameObject(uint idGameObject)
{
	const GameObject* baseGameObject = gameObjects[idGameObject];

	GameObject* gameObject = nullptr;

	switch(baseGameObject->getType())
	{
		case GameObjectType::PLAYER:
			gameObject = getPlayer(idGameObject);
			
			break;
		case GameObjectType::CAR:
			gameObject = getCar(idGameObject);
			
			break;
		case GameObjectType::STATIC:
			gameObject = getStatic(idGameObject);

			break;
		case GameObjectType::ANIMATED:
			gameObject = getAnimated(idGameObject);

			break;
	}

	usedGameObjects.push_back(gameObject);

	return gameObject;
}

bool ModuleGameObject::setUp()
{
	const char* jsonPath = "Resources/Configurations/GameObjects.json";

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	// gameObjects

	const Value& gameObjectsJson = jsonDocument["gameObjects"];

	gameObjects.reserve(gameObjectsJson.Size());

	for(SizeType i = 0; i < gameObjectsJson.Size(); ++i)
		gameObjects.push_back(createGameObject(gameObjectsJson[i]));

	return true;
}

void ModuleGameObject::cleanUp()
{
	for(int i = (int)gameObjects.size() - 1; i >= 0; --i)
	{
		gameObjects[i]->cleanUp();

		delete gameObjects[i]; gameObjects[i] = nullptr;
	}

	gameObjects.clear();

	for(list<GameObject*>::reverse_iterator it = usedGameObjects.rbegin(); it != usedGameObjects.rend(); ++it)
	{
		(*it)->cleanUp();

		delete *it; *it = nullptr;
	}

	usedGameObjects.clear();
}

Car* ModuleGameObject::getCar(uint idGameObject) const
{
	const Car* baseCar = (Car*)gameObjects[idGameObject];

	const AnimationContainer* baseAnimationContainer = baseCar->getAnimationContainer();

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(baseAnimationContainer->getAnimationGroupId(), baseAnimationContainer->getId());

	return new Car(baseCar->getId(), animationContainer);
}

Player* ModuleGameObject::getPlayer(uint idGameObject) const
{
	const Player* basePlayer = (Player*)gameObjects[idGameObject];

	const AnimationContainer* baseAnimationContainer = basePlayer->getAnimationContainer();

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(baseAnimationContainer->getAnimationGroupId(), baseAnimationContainer->getId());

	return new Player(basePlayer->getId(), animationContainer, basePlayer->getModuleInput());
}

Static* ModuleGameObject::getStatic(uint idGameObject) const
{
	return new Static(*(Static*)gameObjects[idGameObject]);
}

Animated* ModuleGameObject::getAnimated(uint idGameObject) const
{
	const Animated* baseAnimated = (Animated*)gameObjects[idGameObject];

	const AnimationContainer* baseAnimationContainer = baseAnimated->getAnimationContainer();

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(baseAnimationContainer->getAnimationGroupId(), baseAnimationContainer->getId());

	return new Animated(baseAnimated->getId(), animationContainer);
}

GameObject* ModuleGameObject::createGameObject(const Value& gameObjectJson) const
{
	// type

	GameObjectType type = (GameObjectType)gameObjectJson["type"].GetInt();

	switch(type)
	{
		case GameObjectType::PLAYER:
			return createPlayer(gameObjectJson);
		case GameObjectType::CAR:
			return createCar(gameObjectJson);
		case GameObjectType::STATIC:
			return createStatic(gameObjectJson);
		case GameObjectType::ANIMATED:
			return createAnimated(gameObjectJson);
	}

	return nullptr;
}

Car* ModuleGameObject::createCar(const rapidjson::Value& gameObjectJson) const
{
	// gameObjectId, animationGroupPath, animationContainerId

	uint gameObjectId = gameObjects.size();
	const char* animationGroupPath = gameObjectJson["animationGroupPath"].GetString();
	uint animationContainerId = gameObjectJson["animationContainerId"].GetInt();

	uint animationGroupId = getGameEngine()->getModuleAnimation()->load(animationGroupPath);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(animationGroupId, animationContainerId);

	return new Car(gameObjectId, animationContainer);
}

Player* ModuleGameObject::createPlayer(const rapidjson::Value& gameObjectJson) const
{
	// gameObjectId, animationGroupPath, animationContainerId

	uint gameObjectId = gameObjects.size();
	const char* animationGroupPath = gameObjectJson["animationGroupPath"].GetString();
	uint animationContainerId = gameObjectJson["animationContainerId"].GetInt();

	uint animationGroupId = getGameEngine()->getModuleAnimation()->load(animationGroupPath);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(animationGroupId, animationContainerId);

	return new Player(gameObjectId, animationContainer, getGameEngine()->getModuleInput());
}

Static* ModuleGameObject::createStatic(const rapidjson::Value& gameObjectJson) const
{
	// gameObjectId, textureGroupPath, textureId

	uint gameObjectId = gameObjects.size();
	const char* textureGroupPath = gameObjectJson["textureGroupPath"].GetString();
	uint textureId = gameObjectJson["textureId"].GetInt();

	uint textureGroupId = getGameEngine()->getModuleTexture()->load(textureGroupPath);

	const Texture* texture = getGameEngine()->getModuleTexture()->get(textureGroupId, textureId);

	return new Static(gameObjectId, texture);
}

Animated* ModuleGameObject::createAnimated(const rapidjson::Value& gameObjectJson) const
{
	// gameObjectId, animationGroupPath, animationContainerId

	uint gameObjectId = gameObjects.size();
	const char* animationGroupPath = gameObjectJson["animationGroupPath"].GetString();
	uint animationContainerId = gameObjectJson["animationContainerId"].GetInt();

	uint animationGroupId = getGameEngine()->getModuleAnimation()->load(animationGroupPath);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(animationGroupId, animationContainerId);

	return new Animated(gameObjectId, animationContainer);
}