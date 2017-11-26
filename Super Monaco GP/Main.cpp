#include "MemLeaks.h"
#include "GameEngine.h"

int main()
{
	ReportMemoryLeaks();

	GameEngine gameEngine;
	gameEngine.run();
}