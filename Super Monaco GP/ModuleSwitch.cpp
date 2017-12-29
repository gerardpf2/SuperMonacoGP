#include "ModuleSwitch.h"

#include "Utils.h"
#include "GameEngine.h"
#include "ModuleRenderer.h"

ModuleSwitch::ModuleSwitch(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleSwitch::~ModuleSwitch()
{ }

void ModuleSwitch::setNewGameModule(GameModule gameModule, float time)
{
	on = true;
	switched = false;
	this->time = time;
	timeSwitch = 0.5f * time;
	this->gameModule = gameModule;
	getGameEngine()->getGameModule()->setBlocked(true);
}

bool ModuleSwitch::update(float deltaTimeS)
{
	if(on)
	{
		if((time -= deltaTimeS) <= 0.0f)
		{
			on = false;
			getGameEngine()->getGameModule()->setBlocked(false);
		}

		if(!switched && time <= timeSwitch)
		{
			switched = true;
			getGameEngine()->setGameModule(gameModule, true);
		}

		float alphaValue;

		if(!switched)
		{
			// t: t -> tS
			// a: 0 -> 1

			alphaValue = 1.0f - (time - timeSwitch) / timeSwitch;
		}
		else
		{
			// t: t -> 0
			// a: 1 -> 0

			alphaValue = time / timeSwitch;
		}

		alphaValue = clamp01(alphaValue);

		getGameEngine()->getModuleRenderer()->setColorAtTop(0, 0, 0, (Uint8)(255.0f * alphaValue));
	}

	return true;
}