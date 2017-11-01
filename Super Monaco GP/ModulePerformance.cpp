#include "ModulePerformance.h"

#include <iostream>

using namespace std;

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

		cout << "ModulePerformance::update" << endl;
		cout << "\t(S) Current: " << deltaTimeS << ", Average: " << sumDeltaTimeS / updateTicks << endl;
		cout << "\t(FPS) Current: " << 1.0f / deltaTimeS << ", Average: " << 1.0f / (sumDeltaTimeS / updateTicks) << endl;
	}

	return true;
}