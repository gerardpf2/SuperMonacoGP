#ifndef _GAME_ENGINE_
#define _GAME_ENGINE_

#include <vector>
#include "Timer.h"

using namespace std;

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

		void run();

		ModuleJson* getModuleJson() const;

		ModuleInput* getModuleInput() const;

		ModuleWorld* getModuleWorld() const;

		ModuleWindow* getModuleWindow() const;

		ModuleTexture* getModuleTexture() const;

		ModuleRenderer* getModuleRenderer() const;

		ModulePerformance* getModulePerformance() const;

	private:

		void addInitialModules();

		bool setUp();

		bool mainLoop();

		void cleanUp();

	private:

		Timer timer;

		vector<Module*> modules;

		ModuleJson* moduleJson = nullptr;

		ModuleInput* moduleInput = nullptr;

		ModuleWorld* moduleWorld = nullptr;

		ModuleWindow* moduleWindow = nullptr;

		ModuleTexture* moduleTexture = nullptr;

		ModuleRenderer* moduleRenderer = nullptr;

		ModulePerformance* modulePerformance = nullptr;
};

#endif