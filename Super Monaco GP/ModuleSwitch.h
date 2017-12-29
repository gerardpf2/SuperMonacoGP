#ifndef _MODULE_SWITCH_
#define _MODULE_SWITCH_

#include "Module.h"
#include "GameEngine.h"

class ModuleSwitch : public Module
{
	public:

		ModuleSwitch(GameEngine* gameEngine);

		virtual ~ModuleSwitch();

		void setNewGameModule(GameModule gameModule, float time = 1.0f);

		virtual bool update(float deltaTimeS) override;

	private:

		bool on = false;

		bool switched = false;

		float time = 0.0f;

		float timeSwitch = 0.0f;

		GameModule gameModule;
};

#endif