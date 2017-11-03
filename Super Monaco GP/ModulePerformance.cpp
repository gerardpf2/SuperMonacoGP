#include "ModulePerformance.h"

#include <stdio.h>

ModulePerformance::ModulePerformance(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModulePerformance::~ModulePerformance()
{ }

bool ModulePerformance::update(float deltaTimeS)
{
	++updateTicks;
	sumDeltaTimeS += deltaTimeS;

	if((printTimeS += deltaTimeS) >= 1.0f)
	{
		printTimeS = 0.0f;

		printf("ModulePerformance::update\n");
		printf("\t(S) Current: %.4f, Average: %.4f\n", deltaTimeS, sumDeltaTimeS / updateTicks);
		printf("\t(FPS) Current: %.4f, Average: %.4f\n", 1.0f / deltaTimeS, 1.0f / (sumDeltaTimeS / updateTicks));
	}

	return true;
}