#include "ModuleGameObject.h"

#include "Player.h"
#include "Static.h"
#include "ModuleJson.h"
#include "GameEngine.h"
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
	assert(gameObjects.size() > idGameObject);

	const GameObject* baseGameObject = gameObjects[idGameObject].first;

	assert(baseGameObject);

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

bool ModuleGameObject::load()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleJson());

	const char* jsonPath = "Resources/Configurations/GameObjects.json";

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	// gameObjects

	const Value& gameObjectsJson = jsonDocument["gameObjects"];

	gameObjects.reserve(gameObjectsJson.Size());

	for(SizeType i = 0; i < gameObjectsJson.Size(); ++i)
	{
		uint resourceGroupId;

		GameObject* gameObject = createGameObject(gameObjectsJson[i], resourceGroupId);
		gameObjects.push_back(pair<GameObject*, uint>(gameObject, resourceGroupId));
	}

	return true;
}

void ModuleGameObject::unload()
{
	assert(getGameEngine());

	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();
	ModuleAnimation* moduleAnimation = getGameEngine()->getModuleAnimation();

	assert(moduleTexture);
	assert(moduleAnimation);

	for(int i = (int)gameObjects.size() - 1; i >= 0; --i)
	{
		GameObject*& gameObject = gameObjects[i].first;

		assert(gameObject);

		gameObject->cleanUp();

		if(gameObject->getType() == GameObjectType::STATIC)
			moduleTexture->unload(gameObjects[i].second);
		else
			moduleAnimation->unload(gameObjects[i].second);

		delete gameObject; gameObject = nullptr;
	}

	gameObjects.clear();

	for(list<GameObject*>::reverse_iterator it = usedGameObjects.rbegin(); it != usedGameObjects.rend(); ++it)
	{
		assert(*it);

		(*it)->cleanUp();

		delete *it; *it = nullptr;
	}

	usedGameObjects.clear();
}

Car* ModuleGameObject::getCar(uint idGameObject) const
{
	assert(getGameEngine());
	assert(gameObjects.size() > idGameObject);
	assert(getGameEngine()->getModuleAnimation());

	const Car* baseCar = (Car*)gameObjects[idGameObject].first;

	assert(baseCar);
	assert(baseCar->getBox());

	const AnimationContainer* baseAnimationContainer = baseCar->getAnimationContainer();

	assert(baseAnimationContainer);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(baseAnimationContainer->getAnimationGroupId(), baseAnimationContainer->getId());

	Car* car = new Car(animationContainer);
	car->setBox(*baseCar->getBox());

	return car;
}

Player* ModuleGameObject::getPlayer(uint idGameObject) const
{
	assert(getGameEngine());
	assert(gameObjects.size() > idGameObject);
	assert(getGameEngine()->getModuleAnimation());

	const Player* basePlayer = (Player*)gameObjects[idGameObject].first;

	assert(basePlayer);
	assert(basePlayer->getBox());

	const AnimationContainer* baseAnimationContainer = basePlayer->getAnimationContainer();

	assert(baseAnimationContainer);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(baseAnimationContainer->getAnimationGroupId(), baseAnimationContainer->getId());

	Player* player = new Player(animationContainer, basePlayer->getModuleInput());
	player->setBox(*basePlayer->getBox());

	return player;
}

Static* ModuleGameObject::getStatic(uint idGameObject) const
{
	assert(gameObjects.size() > idGameObject);

	return new Static(*(Static*)gameObjects[idGameObject].first);
}

Animated* ModuleGameObject::getAnimated(uint idGameObject) const
{
	assert(getGameEngine());
	assert(gameObjects.size() > idGameObject);
	assert(getGameEngine()->getModuleAnimation());

	const Animated* baseAnimated = (Animated*)gameObjects[idGameObject].first;

	assert(baseAnimated);
	assert(baseAnimated->getBox());

	const AnimationContainer* baseAnimationContainer = baseAnimated->getAnimationContainer();

	assert(baseAnimationContainer);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(baseAnimationContainer->getAnimationGroupId(), baseAnimationContainer->getId());

	Animated* animated = new Animated(animationContainer);
	animated->setBox(*baseAnimated->getBox());

	return animated;
}

GameObject* ModuleGameObject::createGameObject(const Value& gameObjectJson, uint &resourceGroupId) const
{
	// type

	GameObjectType type = (GameObjectType)gameObjectJson["type"].GetInt();

	switch(type)
	{
		case GameObjectType::PLAYER:
			return createPlayer(gameObjectJson, resourceGroupId);
		case GameObjectType::CAR:
			return createCar(gameObjectJson, resourceGroupId);
		case GameObjectType::STATIC:
			return createStatic(gameObjectJson, resourceGroupId);
		case GameObjectType::ANIMATED:
			return createAnimated(gameObjectJson, resourceGroupId);
	}

	return nullptr;
}

Car* ModuleGameObject::createCar(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAnimation());

	// animationGroupPath, animationContainerId

	const char* animationGroupPath = gameObjectJson["animationGroupPath"].GetString();
	uint animationContainerId = gameObjectJson["animationContainerId"].GetInt();
	const Value& colliderJson = gameObjectJson["collider"];

	float mW = colliderJson["mW"].GetFloat();
	float d = colliderJson["d"].GetFloat();

	resourceGroupId = getGameEngine()->getModuleAnimation()->load(animationGroupPath);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(resourceGroupId, animationContainerId);

	Car* car = new Car(animationContainer);
	car->defineBox(mW, d);

	return car;
}

Player* ModuleGameObject::createPlayer(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAnimation());

	// animationGroupPath, animationContainerId

	const char* animationGroupPath = gameObjectJson["animationGroupPath"].GetString();
	uint animationContainerId = gameObjectJson["animationContainerId"].GetInt();
	const Value& colliderJson = gameObjectJson["collider"];

	float mW = colliderJson["mW"].GetFloat();
	float d = colliderJson["d"].GetFloat();

	resourceGroupId = getGameEngine()->getModuleAnimation()->load(animationGroupPath);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(resourceGroupId, animationContainerId);

	Player* player = new Player(animationContainer, getGameEngine()->getModuleInput());
	player->defineBox(mW, d);

	return player;
}

Static* ModuleGameObject::createStatic(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleTexture());

	// textureGroupPath, textureId

	const char* textureGroupPath = gameObjectJson["textureGroupPath"].GetString();
	uint textureId = gameObjectJson["textureId"].GetInt();
	const Value& colliderJson = gameObjectJson["collider"];

	float mW = colliderJson["mW"].GetFloat();
	float d = colliderJson["d"].GetFloat();

	resourceGroupId = getGameEngine()->getModuleTexture()->load(textureGroupPath);

	const Texture* texture = getGameEngine()->getModuleTexture()->get(resourceGroupId, textureId);

	Static* static_ = new Static(texture);
	static_->defineBox(mW, d);

	return static_;
}

Animated* ModuleGameObject::createAnimated(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAnimation());

	// animationGroupPath, animationContainerId

	const char* animationGroupPath = gameObjectJson["animationGroupPath"].GetString();
	uint animationContainerId = gameObjectJson["animationContainerId"].GetInt();
	const Value& colliderJson = gameObjectJson["collider"];

	float mW = colliderJson["mW"].GetFloat();
	float d = colliderJson["d"].GetFloat();

	resourceGroupId = getGameEngine()->getModuleAnimation()->load(animationGroupPath);

	AnimationContainer* animationContainer = getGameEngine()->getModuleAnimation()->getAnimationContainer(resourceGroupId, animationContainerId);

	Animated* animated = new Animated(animationContainer);
	animated->defineBox(mW, d);

	return animated;
}