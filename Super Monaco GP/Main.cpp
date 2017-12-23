#include "MemLeaks.h"
#include "GameEngine.h"
#include "ModuleStart.h"
// #include "ModuleWorld.h"

int main()
{
	ReportMemoryLeaks();

	GameEngine gameEngine;
	gameEngine.run(new ModuleStart(&gameEngine));
	// gameEngine.run(new ModuleWorld(&gameEngine));
}