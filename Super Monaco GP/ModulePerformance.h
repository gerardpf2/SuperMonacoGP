#ifndef _MODULE_PERFORMANCE_
#define _MODULE_PERFORMANCE_

#include "Module.h"

class ModulePerformance : public Module
{
	public:

		ModulePerformance(GameEngine* gameEngine, bool active = true);

		virtual ~ModulePerformance();

		virtual bool update(float deltaTimeS) override;

	private:

		float printTimeS = 0.0f;

		long double sumDeltaTimeS = 0.0;

		unsigned long long updateTicks = 0;
};

#endif