#include "ModuleSuperMonacoGP.h"

#include "Car.h"
#include "Road.h"
#include "Utils.h"
#include "Player.h"
#include "GameEngine.h"
#include "ModuleFont.h"
#include "ModuleSwitch.h"
#include "ModuleRegistry.h"
#include "ModuleSuperMonacoGPUI.h"

using namespace std;

ModuleSuperMonacoGP::ModuleSuperMonacoGP(GameEngine* gameEngine) :
	ModuleWorld(gameEngine)
{
	lapTimesPosition = LAP_TIMES_POSITION;

	positionPosition = POSITION_POSITION;

	positionValuePosition = POSITION_VALUE_POSITION;

	currentLapPosition = CURRENT_LAP_POSITION;

	// currentLapValuePosition = CURRENT_LAP_VALUE_POSITION;
}

ModuleSuperMonacoGP::~ModuleSuperMonacoGP()
{ }

bool ModuleSuperMonacoGP::setUp()
{
	bool ret = ModuleWorld::setUp();

	lapsCars.resize(N_CARS, 0);

	getGameEngine()->getModuleRegistry()->defaultValues();

	return ret;
}

bool ModuleSuperMonacoGP::update(float deltaTimeS)
{
	bool ret = ModuleWorld::update(deltaTimeS);

	if(!getBlocked() && !paused && goResults)
	{
		if((waitTimeGoResults += deltaTimeS) >= WAIT_TIME_GO_RESULTS)
		{
			// getGameEngine()->setGameModule(GameModule::RESULTS);
			getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::RESULTS);

			registerEstimatedLapTimes();
		}
	}

	return ret;
}

void ModuleSuperMonacoGP::cleanUp()
{
	ModuleWorld::cleanUp();

	lapsCars.clear();

	enemyCars.clear();
}

void ModuleSuperMonacoGP::registerEstimatedLapTimes() const
{
	// Some enemy cars may not have completed all laps
	// But, in order to provide some results, its necessary to know their lap times
	// Given an enemy car, for the first missing lap, compute an estimated lap time based on the position of that enemy car and the corresponding lap time of the player
	// For the rest, assign the lap time of the player

	uint playerId = player->getSpecificId();
	float roadLength = getRoad()->getLength();
	ModuleRegistry* moduleRegistry = getGameEngine()->getModuleRegistry();

	for(const Car* enemyCar : enemyCars)
	{
		uint id = enemyCar->getSpecificId();
		uint lastLap = lapsCars[id];

		if(lastLap < N_LAPS)
		{
			float lapTime = enemyCar->getCurrentLapTime();
			lapTime += interpolate(enemyCar->getPosition()->z, 0.0f, roadLength, moduleRegistry->getCarLapTime(playerId, lastLap), 0.0f);

			moduleRegistry->setCarLapTime(id, lastLap, lapTime);

			for(uint i = ++lastLap; i < N_LAPS; ++i)
				moduleRegistry->setCarLapTime(id, i, moduleRegistry->getCarLapTime(playerId, i));
		}
	}
}

void ModuleSuperMonacoGP::registerLapTime(const Car* car)
{
	if(lapsCars[car->getSpecificId()] >= N_LAPS) return;

	ModuleWorld::registerLapTime(car);

	getGameEngine()->getModuleRegistry()->updateCarLapTimeIfBetter(car->getSpecificId(), car->getCurrentLap(), car->getCurrentLapTime());

	if(car->getType() == GameObjectType::PLAYER)
		getGameEngine()->getModuleRegistry()->updatePlayerBestLapTimeIfBetter(car->getCurrentLapTime());

	if((++lapsCars[car->getSpecificId()]) == N_LAPS && car->getType() == GameObjectType::PLAYER)
	{
		goResults = true;
		// getGameEngine()->setGameModule(GameModule::RESULTS);

		renderCurrentLapTime = false;
	}
}

void ModuleSuperMonacoGP::addPlayer()
{
	float z = -PLAYER_POSITION * CARS_OFFSET_Z;
	float sidePositionX = (PLAYER_POSITION % 2 == 0) ? 1.0f : -1.0f;
	
	player = (Player*)addGameObject(0, WorldPosition{ sidePositionX * CARS_X, 0.0f, z });

	// player = (Player*)addGameObject(0, WorldPosition{ 7.5f, 0.0f, 300.0f });
}

void ModuleSuperMonacoGP::addCars()
{
	// enemyCars.push_back((Car*)addGameObject(1, WorldPosition{ 7.5f, 0.0f, 0.0f }));

	// return;

	float z = -SEGMENT_LENGTH;
	float sidePositionX = 1.0f;

	for(uint i = 0; i < N_CARS; ++i)
	{
		if(i != PLAYER_POSITION) enemyCars.push_back((Car*)addGameObject(1, WorldPosition{ sidePositionX * CARS_X, 0.0f, z }));

		z -= CARS_OFFSET_Z;
		sidePositionX *= -1.0f;
	}
}

void ModuleSuperMonacoGP::renderUI() const
{
	ModuleWorld::renderUI();

	if(player->getMovementEnabled())
		renderLapTimes();

	renderCurrentPosition();
}

void ModuleSuperMonacoGP::renderLapTimes() const
{
	getGameEngine()->getModuleFont()->renderText("LAP", lapTimesPosition, HAlignment::LEFT, VAlignment::BOTTOM, LAP_TIMES_POSITION_SCALE, LAP_TIMES_POSITION_SCALE, 224, 160, 0);

	uint laps = player->getCurrentLap() + 1;
	if(laps > N_LAPS) laps = N_LAPS;

	string currentLapValueStr = to_string(laps);
	currentLapValueStr += "/";
	currentLapValueStr += to_string(N_LAPS);

	getGameEngine()->getModuleFont()->renderText(currentLapValueStr, currentLapPosition, HAlignment::LEFT, VAlignment::BOTTOM, CURRENT_LAP_POSITION_SCALE, CURRENT_LAP_POSITION_SCALE, 248, 36, 32);

	float y = LAP_TIMES_VALUE_POSITION_Y;

	for(uint i = 0; i < N_LAPS; ++i)
	{
		string lapTimeStr;
		time(getGameEngine()->getModuleRegistry()->getCarLapTime(player->getSpecificId(), i), lapTimeStr);

		Uint8 r, g, b;

		if(i < lapsCars[player->getSpecificId()])
		{
			r = 224; g = 160; b = 0;
		}
		else
		{
			r = 248; g = 252; b = 248;
		}

		getGameEngine()->getModuleFont()->renderText(lapTimeStr, { LAP_TIMES_POSITION_X, (short)y }, HAlignment::LEFT, VAlignment::BOTTOM, LAP_TIMES_VALUE_POSITION_SCALE, LAP_TIMES_VALUE_POSITION_SCALE, r, g, b);

		y += LAP_TIMES_VALUE_POSITION_H;
	}
}

/* void ModuleSuperMonacoGP::renderCurrentLap() const
{
	getGameEngine()->getModuleFont()->renderText("LAP", currentLapPosition, HAlignment::LEFT, VAlignment::BOTTOM, CURRENT_LAP_POSITION_SCALE, CURRENT_LAP_POSITION_SCALE, 224, 160, 0);

	uint laps = player->getCurrentLap() + 1;
	if(laps > N_LAPS) laps = N_LAPS;

	string currentLapValueStr = to_string(laps);
	currentLapValueStr += "/";
	currentLapValueStr += to_string(N_LAPS);

	getGameEngine()->getModuleFont()->renderText(currentLapValueStr, currentLapValuePosition, HAlignment::CENTER, VAlignment::BOTTOM, CURRENT_LAP_VALUE_POSITION_SCALE, CURRENT_LAP_VALUE_POSITION_SCALE, 248, 36, 32);
} */

void ModuleSuperMonacoGP::renderCurrentPosition() const
{
	getGameEngine()->getModuleFont()->renderText("POSITION", positionPosition, HAlignment::LEFT, VAlignment::BOTTOM, POSITION_POSITION_SCALE, POSITION_POSITION_SCALE, 224, 160, 0);

	uint position = 1;

	for(const Car* enemyCar : enemyCars)
		if(enemyCar->getGlobalZ() > player->getGlobalZ()) ++position;

	getGameEngine()->getModuleFont()->renderText(to_string(position), positionValuePosition, HAlignment::CENTER, VAlignment::BOTTOM, POSITION_VALUE_POSITION_SCALE, POSITION_VALUE_POSITION_SCALE, 248, 36, 32);
}