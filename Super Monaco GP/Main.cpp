#include "MemLeaks.h"
#include "GameEngine.h"

int main()
{
	ReportMemoryLeaks();

	GameEngine gameEngine;

	gameEngine.run(GameModule::SEGA);
	// gameEngine.run(GameModule::START);
	// gameEngine.run(GameModule::COURSE_SELECT);
	// gameEngine.run(GameModule::FREE_PRACTICE);
	// gameEngine.run(GameModule::SUPER_MONACO_GP);
	// gameEngine.run(GameModule::RESULTS);
	// gameEngine.run(GameModule::ABOUT);
}