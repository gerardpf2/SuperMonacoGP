#include "ModuleFreePractice.h"

#include "Car.h"
#include "Utils.h"
#include "GameEngine.h"
#include "ModuleFont.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"
#include "ModuleFreePracticeUI.h"

ModuleFreePractice::ModuleFreePractice(GameEngine* gameEngine) :
	ModuleWorld(gameEngine)
{
	freePracticePosition = FREE_PRACTICE_POSITION;
	freePracticeFlag0Rect = FREE_PRACTICE_FLAG_0_RECT;
	freePracticeFlag1Rect = FREE_PRACTICE_FLAG_1_RECT;
}

ModuleFreePractice::~ModuleFreePractice()
{ }

bool ModuleFreePractice::setUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleTexture());

	bool ret = ModuleWorld::setUp();

	flag0 = getGameEngine()->getModuleTexture()->get(2, 14);
	flag1 = getGameEngine()->getModuleTexture()->get(2, 15);

	return ret;
}

bool ModuleFreePractice::update(float deltaTimeS)
{
	bool ret = ModuleWorld::update(deltaTimeS);

	if(!getBlocked() && !paused)
		updateFreePracticeCounter(deltaTimeS);

	return ret;
}

void ModuleFreePractice::cleanUp()
{
	ModuleWorld::cleanUp();

	flag0 = flag1 = nullptr;
}

void ModuleFreePractice::registerLapTime(const Car* car)
{
	assert(car);

	ModuleWorld::registerLapTime(car);

	if(car->getType() == GameObjectType::PLAYER)
	{
		assert(getGameEngine());
		assert(getGameEngine()->getModuleRegistry());

		getGameEngine()->getModuleRegistry()->updatePlayerBestLapTimeIfBetter(car->getCurrentLapTime());
	}
}

void ModuleFreePractice::renderUI() const
{
	ModuleWorld::renderUI();

	if(freePracticeCounter <= 0.5f)
	{
		assert(flag0);
		assert(flag1);
		assert(getGameEngine());
		assert(getGameEngine()->getModuleFont());
		assert(getGameEngine()->getModuleRenderer());

		getGameEngine()->getModuleRenderer()->renderTexture(flag0->t, flag0->r, &freePracticeFlag0Rect, flag0->hFlipped);
		getGameEngine()->getModuleRenderer()->renderTexture(flag1->t, flag1->r, &freePracticeFlag1Rect, flag1->hFlipped);
		getGameEngine()->getModuleFont()->renderText("FREE PRACTICE", freePracticePosition, HAlignment::RIGHT, VAlignment::BOTTOM, FREE_PRACTICE_POSITION_SCALE, FREE_PRACTICE_POSITION_SCALE, 248, 36, 32);
	}
}

void ModuleFreePractice::updateFreePracticeCounter(float deltaTimeS)
{
	freePracticeCounter = mod0L(freePracticeCounter + 0.75f * deltaTimeS, 1.0f);
}