#include "GameEngine.h"

#include "ModuleJson.h"
#include "ModuleInput.h"
#include "ModuleWorld.h"
#include "ModuleWindow.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"
#include "ModuleAnimation.h"
#include "ModuleGameObject.h" //
#include "ModulePerformance.h"

GameEngine::GameEngine()
{ }

GameEngine::~GameEngine()
{ }

ModuleJson* GameEngine::getModuleJson() const
{
	return moduleJson;
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

ModuleTexture* GameEngine::getModuleTexture() const
{
	return moduleTexture;
}

ModuleRenderer* GameEngine::getModuleRenderer() const
{
	return moduleRenderer;
}

ModuleAnimation* GameEngine::getModuleAnimation() const
{
	return moduleAnimation;
}

ModuleGameObject* GameEngine::getModuleGameObject() const //
{
	return moduleGameObject;
}

ModulePerformance* GameEngine::getModulePerformance() const
{
	return modulePerformance;
}

void GameEngine::run()
{
	if(setUp())
		while(mainLoop());

	cleanUp();
}

void GameEngine::addInitialModules()
{
	modules.reserve(8);

	modules.push_back(moduleJson = new ModuleJson(this));
	modules.push_back(moduleInput = new ModuleInput(this));
	modules.push_back(moduleWindow = new ModuleWindow(this));
	modules.push_back(moduleRenderer = new ModuleRenderer(this));
	modules.push_back(moduleTexture = new ModuleTexture(this));
	modules.push_back(moduleAnimation = new ModuleAnimation(this));
	modules.push_back(moduleGameObject = new ModuleGameObject(this)); //
	// modules.push_back(modulePerformance = new ModulePerformance(this));

	modules.push_back(moduleWorld = new ModuleWorld(this));
}

bool GameEngine::setUp()
{
	addInitialModules();

	bool setUp = true;

	for(uint i = 0; setUp && i < (uint)modules.size(); ++i)
		setUp = modules[i]->setUp();

	timer.start();

	return setUp;
}

bool GameEngine::mainLoop()
{
	float deltaTimeS = timer.getElapsedTimeMs() / 1000.0f;

	timer.start();

	bool mainLoop = true;

	for(uint i = 0; mainLoop && i < (uint)modules.size(); ++i)
		mainLoop = modules[i]->preUpdate(deltaTimeS);

	for(uint i = 0; mainLoop && i < (uint)modules.size(); ++i)
		mainLoop = modules[i]->update(deltaTimeS);

	for(uint i = 0; mainLoop && i < (uint)modules.size(); ++i)
		mainLoop = modules[i]->postUpdate(deltaTimeS);

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

	moduleJson = nullptr;
	moduleInput = nullptr;
	moduleWorld = nullptr;
	moduleWindow = nullptr;
	moduleTexture = nullptr;
	moduleRenderer = nullptr;
	moduleAnimation = nullptr;
	moduleGameObject = nullptr; //
	modulePerformance = nullptr;
}