#ifndef _GAME_ENGINE_
#define _GAME_ENGINE_

#include <vector>
#include "Timer.h"

class Module;
class ModuleJson;
class ModuleInput;
class ModuleWorld;
class ModuleWindow;
class ModuleTexture;
class ModuleRenderer;
class ModuleAnimation;
class ModuleGameObject; //
class ModulePerformance;

class GameEngine
{
	public:

		GameEngine();

		virtual ~GameEngine();

		ModuleJson* getModuleJson() const;

		ModuleInput* getModuleInput() const;

		ModuleWorld* getModuleWorld() const;

		ModuleWindow* getModuleWindow() const;

		ModuleTexture* getModuleTexture() const;

		ModuleRenderer* getModuleRenderer() const;

		ModuleAnimation* getModuleAnimation() const;

		ModuleGameObject* getModuleGameObject() const; //

		ModulePerformance* getModulePerformance() const;

		void run();

	private:

		void addInitialModules();

		bool setUp();

		bool mainLoop();

		void cleanUp();

	private:

		Timer timer;

		std::vector<Module*> modules;

		ModuleJson* moduleJson = nullptr;

		ModuleInput* moduleInput = nullptr;

		ModuleWorld* moduleWorld = nullptr;

		ModuleWindow* moduleWindow = nullptr;

		ModuleTexture* moduleTexture = nullptr;

		ModuleRenderer* moduleRenderer = nullptr;

		ModuleAnimation* moduleAnimation = nullptr;

		ModuleGameObject* moduleGameObject = nullptr; //

		ModulePerformance* modulePerformance = nullptr;
};

#endif