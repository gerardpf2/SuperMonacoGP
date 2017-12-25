#include "ModuleSuperMonacoGP.h"

#include "Car.h"
#include "GameEngine.h"
#include "ModuleRegistry.h"

ModuleSuperMonacoGP::ModuleSuperMonacoGP(GameEngine* gameEngine) :
	ModuleWorld(gameEngine)
{ }

ModuleSuperMonacoGP::~ModuleSuperMonacoGP()
{ }

bool ModuleSuperMonacoGP::setUp()
{
	bool ret = ModuleWorld::setUp();

	return ret;
}

bool ModuleSuperMonacoGP::update(float deltaTimeS)
{
	bool ret = ModuleWorld::update(deltaTimeS);

	return ret;
}

void ModuleSuperMonacoGP::cleanUp()
{
	ModuleWorld::cleanUp();
}

void ModuleSuperMonacoGP::registerLapTime(const Car* car) const
{
	getGameEngine()->getModuleRegistry()->updateCarLapTimeIfBetter(car->getSpecificId(), car->getCurrentLap(), car->getCurrentLapTime());

	if(car->getType() == GameObjectType::PLAYER)
		getGameEngine()->getModuleRegistry()->updatePlayerBestLapTimeIfBetter(car->getCurrentLapTime());
}

void ModuleSuperMonacoGP::addPlayer()
{
	player = (Player*)addGameObject(0, WorldPosition{ 0.0f, 0.0f, -25.0f });
}

void ModuleSuperMonacoGP::addCars()
{
	addGameObject(1, WorldPosition{ 7.5f, 0.0f, -5.0f });
	addGameObject(1, WorldPosition{ -7.5f, 0.0f, -10.0f });
	addGameObject(1, WorldPosition{ 7.5f, 0.0f, -15.0f });
	addGameObject(1, WorldPosition{ -7.5f, 0.0f, -20.0f });
}