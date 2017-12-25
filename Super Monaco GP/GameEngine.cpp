#include "GameEngine.h"

#include <time.h>
#include "ModuleJson.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleWorld.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"
#include "ModuleAnimation.h"
#include "ModuleCollision.h"
#include "ModuleGameObject.h"
#include "ModulePerformance.h"

GameEngine::GameEngine()
{
	srand((uint)time(nullptr));
}

GameEngine::~GameEngine()
{ }

ModuleJson* GameEngine::getModuleJson() const
{
	return moduleJson;
}

ModuleFont* GameEngine::getModuleFont() const
{
	return moduleFont;
}

ModuleInput* GameEngine::getModuleInput() const
{
	return moduleInput;
}

/* ModuleWorld* GameEngine::getModuleWorld() const
{
	return moduleWorld;
} */

ModuleWindow* GameEngine::getModuleWindow() const
{
	return moduleWindow;
}

ModuleTexture* GameEngine::getModuleTexture() const
{
	return moduleTexture;
}

ModuleRenderer* GameEngine::getModuleRenderer() const
{
	return moduleRenderer;
}

ModuleRegistry* GameEngine::getModuleRegistry() const
{
	return moduleRegistry;
}

ModuleAnimation* GameEngine::getModuleAnimation() const
{
	return moduleAnimation;
}

ModuleCollision* GameEngine::getModuleCollision() const
{
	return moduleCollision;
}

ModuleGameObject* GameEngine::getModuleGameObject() const
{
	return moduleGameObject;
}

ModulePerformance* GameEngine::getModulePerformance() const
{
	return modulePerformance;
}

Module* GameEngine::getGameModule() const
{
	return gameModule;
}

void GameEngine::setGameModule(Module* gameModule)
{
	tmpGameModule = gameModule;
}

void GameEngine::run(Module* initialGameModule)
{
	setGameModule(initialGameModule);

	if(setUp())
		while(mainLoop())
			if(!setUpNewGameModule()) break;

	cleanUp();
}

void GameEngine::addInitialModules()
{
	modules.reserve(11); // 9 (core modules) + 1 (game module)

	modules.push_back(moduleRegistry = new ModuleRegistry(this));
	modules.push_back(moduleJson = new ModuleJson(this));
	modules.push_back(moduleInput = new ModuleInput(this));
	modules.push_back(moduleWindow = new ModuleWindow(this));
	modules.push_back(moduleRenderer = new ModuleRenderer(this));
	modules.push_back(moduleTexture = new ModuleTexture(this));
	modules.push_back(moduleFont = new ModuleFont(this));
	modules.push_back(moduleAnimation = new ModuleAnimation(this));
	modules.push_back(moduleCollision = new ModuleCollision(this));
	modules.push_back(moduleGameObject = new ModuleGameObject(this));
	// modules.push_back(modulePerformance = new ModulePerformance(this));

	modules.push_back(gameModule = nullptr);
	// modules.push_back(moduleWorld = new ModuleWorld(this));
}

bool GameEngine::setUp()
{
	addInitialModules();

	bool setUp = true;

	for(uint i = 0; setUp && i < (uint)modules.size(); ++i)
		if(modules[i]) setUp = modules[i]->setUp();

	timer.start();

	return setUp;
}

bool GameEngine::mainLoop()
{
	float deltaTimeS = timer.getElapsedTimeMs() / 1000.0f;

	timer.start();

	bool mainLoop = true;

	for(uint i = 0; mainLoop && i < (uint)modules.size(); ++i)
		if(modules[i]) mainLoop = modules[i]->preUpdate(deltaTimeS);

	for(uint i = 0; mainLoop && i < (uint)modules.size(); ++i)
		if(modules[i]) mainLoop = modules[i]->update(deltaTimeS);

	for(uint i = 0; mainLoop && i < (uint)modules.size(); ++i)
		if(modules[i]) mainLoop = modules[i]->postUpdate(deltaTimeS);

	return mainLoop;
}

bool GameEngine::setUpNewGameModule()
{
	if(tmpGameModule)
	{
		if(gameModule)
		{
			gameModule->cleanUp();
			delete gameModule; gameModule = nullptr;
		}

		modules[modules.size() - 1] = gameModule = tmpGameModule;
		
		tmpGameModule = nullptr;

		return gameModule->setUp();
	}

	return true;
}

void GameEngine::cleanUp()
{
	for(int i = (int)modules.size() - 1; i >= 0; --i)
	{
		if(modules[i])
		{
			modules[i]->cleanUp();
			delete modules[i]; modules[i] = nullptr;
		}
	}

	modules.clear();

	moduleJson = nullptr;
	moduleFont = nullptr;
	moduleInput = nullptr;
	// moduleWorld = nullptr;
	moduleWindow = nullptr;
	moduleTexture = nullptr;
	moduleRenderer = nullptr;
	moduleRegistry = nullptr;
	moduleAnimation = nullptr;
	moduleCollision = nullptr;
	moduleGameObject = nullptr;
	modulePerformance = nullptr;

	gameModule = tmpGameModule = nullptr;
}