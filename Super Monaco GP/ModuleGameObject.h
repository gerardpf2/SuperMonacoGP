#ifndef _MODULE_GAME_OBJECT_
#define _MODULE_GAME_OBJECT_

#include <list>
#include <vector>
#include "Types.h"
#include "Module.h"
#include "rapidjson/document.h"

class Car;
class Player;
class Static;
class Animated;
class GameObject;

class ModuleGameObject : public Module
{
	public:

		ModuleGameObject(GameEngine* gameEngine);

		virtual ~ModuleGameObject();

		GameObject* getGameObject(uint idGameObject);

		bool load();

		void unload();

	private:

		Car* getCar(uint idGameObject) const;

		Player* getPlayer(uint idGameObject) const;

		Static* getStatic(uint idGameObject) const;

		Animated* getAnimated(uint idGameObject) const;

		GameObject* createGameObject(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const;

		Car* createCar(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const;

		Player* createPlayer(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const;

		Static* createStatic(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const;

		Animated* createAnimated(const rapidjson::Value& gameObjectJson, uint &resourceGroupId) const;

	private:

		std::vector<std::pair<GameObject*, uint>> gameObjects;

		std::list<GameObject*> usedGameObjects;
};

#endif