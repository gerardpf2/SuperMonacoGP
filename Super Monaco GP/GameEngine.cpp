#include "GameEngine.h"

#include <time.h>
#include "ModuleJson.h"
#include "ModuleFont.h"
#include "ModuleSega.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleStart.h"
#include "ModuleAbout.h"
#include "ModuleWindow.h"
#include "ModuleSwitch.h"
#include "ModuleTexture.h"
#include "ModuleResults.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"
#include "ModuleAnimation.h"
#include "ModuleCollision.h"
#include "ModuleGameObject.h"
#include "ModulePerformance.h"
#include "ModuleCourseSelect.h"
#include "ModuleFreePractice.h"
#include "ModuleSuperMonacoGP.h"

GameEngine::GameEngine()
{
	srand((uint)time(nullptr));
}

GameEngine::~GameEngine()
{ }

Module* GameEngine::getGameModule() const
{
	return gameModule;
}

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

ModuleAudio* GameEngine::getModuleAudio() const
{
	return moduleAudio;
}

ModuleWindow* GameEngine::getModuleWindow() const
{
	return moduleWindow;
}

ModuleSwitch* GameEngine::getModuleSwitch() const
{
	return moduleSwitch;
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

void GameEngine::setGameModule(GameModule gameModule, bool blocked)
{
	switch(gameModule)
	{
		case GameModule::SEGA:
			tmpGameModule = new ModuleSega(this);

			break;
		case GameModule::START:
			tmpGameModule = new ModuleStart(this);

			break;
		case GameModule::COURSE_SELECT:
			tmpGameModule = new ModuleCourseSelect(this);

			break;
		case GameModule::FREE_PRACTICE:
			tmpGameModule = new ModuleFreePractice(this);

			break;
		case GameModule::SUPER_MONACO_GP:
			tmpGameModule = new ModuleSuperMonacoGP(this);

			break;
		case GameModule::RESULTS:
			tmpGameModule = new ModuleResults(this);

			break;
		case GameModule::ABOUT:
			tmpGameModule = new ModuleAbout(this);

			break;
	}

	assert(tmpGameModule);

	tmpGameModule->setBlocked(blocked);
}

void GameEngine::run(GameModule gameModule)
{
	setGameModule(gameModule);

	if(setUp())
		while(mainLoop())
			if(!setUpNewGameModule()) break;

	cleanUp();
}

void GameEngine::addInitialModules()
{
	modules.reserve(13); // 12 (core modules) + 1 (game module)

	modules.push_back(moduleRegistry = new ModuleRegistry(this));
	modules.push_back(moduleJson = new ModuleJson(this));
	modules.push_back(moduleInput = new ModuleInput(this));
	modules.push_back(moduleAudio = new ModuleAudio(this));
	modules.push_back(moduleWindow = new ModuleWindow(this));
	modules.push_back(moduleRenderer = new ModuleRenderer(this));
	modules.push_back(moduleTexture = new ModuleTexture(this));
	modules.push_back(moduleFont = new ModuleFont(this));
	modules.push_back(moduleAnimation = new ModuleAnimation(this));
	modules.push_back(moduleCollision = new ModuleCollision(this));
	modules.push_back(moduleGameObject = new ModuleGameObject(this));
	modules.push_back(moduleSwitch = new ModuleSwitch(this));
	// modules.push_back(modulePerformance = new ModulePerformance(this));

	modules.push_back(gameModule = nullptr);
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

		assert(!modules.empty());

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
	moduleAudio = nullptr;
	moduleWindow = nullptr;
	moduleSwitch = nullptr;
	moduleTexture = nullptr;
	moduleRenderer = nullptr;
	moduleRegistry = nullptr;
	moduleAnimation = nullptr;
	moduleCollision = nullptr;
	moduleGameObject = nullptr;
	modulePerformance = nullptr;
	gameModule = tmpGameModule = nullptr;
}