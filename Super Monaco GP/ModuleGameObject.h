#ifndef _MODULE_GAME_OBJECT_
#define _MODULE_GAME_OBJECT_

#include <list>
#include <vector>
#include "Types.h"
#include "Module.h"
#include "rapidjson/document.h"

class Car;
class Player;
class GameObject;

class ModuleGameObject : public Module
{
	public:

		ModuleGameObject(GameEngine* gameEngine);

		virtual ~ModuleGameObject();

		GameObject* getGameObject(uint idGameObject);

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		Car* getCar(uint idGameObject) const;

		Player* getPlayer(uint idGameObject) const;

		// ...

		GameObject* createGameObject(const rapidjson::Value& gameObjectJson) const;

		Car* createCar(const rapidjson::Value& gameObjectJson) const;

		Player* createPlayer(const rapidjson::Value& gameObjectJson) const;

		GameObject* createEnvironment(const rapidjson::Value& gameObjectJson) const;

		GameObject* createEnvironmentAnimated(const rapidjson::Value& gameObjectJson) const;

	private:

		std::vector<GameObject*> gameObjects;

		std::list<GameObject*> usedGameObjects;
};

#endif