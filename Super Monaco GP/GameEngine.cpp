#include "GameEngine.h"

#include "ModuleInput.h"
#include "ModuleWorld.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "ModulePerformance.h"

GameEngine::GameEngine()
{ }

GameEngine::~GameEngine()
{ }

void GameEngine::run()
{
	if(setUp())
		while(mainLoop());

	cleanUp();
}

ModuleInput* GameEngine::getModuleInput() const
{
	return moduleInput;
}

ModuleWorld* GameEngine::getModuleWorld() const
{
	return moduleWorld;
}

ModuleWindow* GameEngine::getModuleWindow() const
{
	return moduleWindow;
}

ModuleRenderer* GameEngine::getModuleRenderer() const
{
	return moduleRenderer;
}

ModulePerformance* GameEngine::getModulePerformance() const
{
	return modulePerformance;
}

void GameEngine::addInitialModules()
{
	modules.reserve(4);

	modules.push_back(moduleInput = new ModuleInput(this));
	modules.push_back(moduleWorld = new ModuleWorld(this));
	modules.push_back(moduleWindow = new ModuleWindow(this));
	modules.push_back(moduleRenderer = new ModuleRenderer(this));
	modules.push_back(modulePerformance = new ModulePerformance(this));
}

bool GameEngine::setUp()
{
	addInitialModules();

	bool setUp = true;

	for(unsigned int i = 0; setUp && i < (unsigned int)modules.size(); ++i)
		setUp = modules[i]->setUp();

	timer.start();

	return setUp;
}

bool GameEngine::mainLoop()
{
	float deltaTimeS = timer.getElapsedTimeMs() / 1000.0f;

	timer.start();

	bool mainLoop = true;

	for(unsigned int i = 0; mainLoop && i < (unsigned int)modules.size(); ++i)
		if(modules[i]->getActive()) mainLoop = modules[i]->preUpdate();

	for(unsigned int i = 0; mainLoop && i < (unsigned int)modules.size(); ++i)
		if(modules[i]->getActive()) mainLoop = modules[i]->update(deltaTimeS);

	for(unsigned int i = 0; mainLoop && i < (unsigned int)modules.size(); ++i)
		if(modules[i]->getActive()) mainLoop = modules[i]->postUpdate();

	return mainLoop;
}

void GameEngine::cleanUp()
{
	for(int i = (int)modules.size() - 1; i >= 0; --i)
	{
		modules[i]->cleanUp();
		
		delete modules[i];
		modules[i] = nullptr;
	}

	modules.clear();

	moduleInput = nullptr;
	moduleWorld = nullptr;
	moduleWindow = nullptr;
	moduleRenderer = nullptr;
	modulePerformance = nullptr;
}