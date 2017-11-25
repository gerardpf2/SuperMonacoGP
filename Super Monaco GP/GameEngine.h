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
class ModulePerformance;

class GameEngine
{
	public:

		GameEngine();

		virtual ~GameEngine();

		const ModuleJson* getModuleJson() const;

		const ModuleInput* getModuleInput() const;

		const ModuleWorld* getModuleWorld() const;

		const ModuleWindow* getModuleWindow() const;

		const ModuleTexture* getModuleTexture() const;

		const ModuleRenderer* getModuleRenderer() const;

		const ModulePerformance* getModulePerformance() const;

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

		ModulePerformance* modulePerformance = nullptr;
};

#endif