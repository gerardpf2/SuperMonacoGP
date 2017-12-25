#include "MemLeaks.h"
#include "GameEngine.h"
#include "ModuleStart.h"
#include "ModuleFreePractice.h"
#include "ModuleSuperMonacoGP.h"

int main()
{
	ReportMemoryLeaks();

	GameEngine gameEngine;
	gameEngine.run(new ModuleStart(&gameEngine));
	// gameEngine.run(new ModuleFreePractice(&gameEngine));
	// gameEngine.run(new ModuleSuperMonacoGP(&gameEngine));
}