#ifndef _GAME_ENGINE_
#define _GAME_ENGINE_

#include <vector>
#include "Timer.h"

enum class GameModule
{
	START,
	COURSE_SELECT,
	FREE_PRACTICE,
	SUPER_MONACO_GP,
	RESULTS,
	ABOUT,
};

class Module;
class ModuleJson;
class ModuleFont;
class ModuleInput;
// class ModuleStart;
// class ModuleWorld;
class ModuleWindow;
class ModuleTexture;
class ModuleRenderer;
class ModuleRegistry;
class ModuleAnimation;
class ModuleCollision;
class ModuleGameObject;
class ModulePerformance;

class GameEngine
{
	public:

		GameEngine();

		virtual ~GameEngine();

		ModuleJson* getModuleJson() const;

		ModuleFont* getModuleFont() const;

		ModuleInput* getModuleInput() const;

		// ModuleWorld* getModuleWorld() const;

		ModuleWindow* getModuleWindow() const;

		ModuleTexture* getModuleTexture() const;

		ModuleRenderer* getModuleRenderer() const;

		ModuleRegistry* getModuleRegistry() const;

		ModuleAnimation* getModuleAnimation() const;

		ModuleCollision* getModuleCollision() const;

		ModuleGameObject* getModuleGameObject() const;

		ModulePerformance* getModulePerformance() const;

		Module* getGameModule() const;

		void setGameModule(GameModule gameModule);

		void run(GameModule gameModule);

	private:

		void addInitialModules();

		bool setUp();

		bool mainLoop();

		bool setUpNewGameModule();

		void cleanUp();

	private:

		Timer timer;

		std::vector<Module*> modules;

		ModuleJson* moduleJson = nullptr;

		ModuleFont* moduleFont = nullptr;

		ModuleInput* moduleInput = nullptr;

		// ModuleWorld* moduleWorld = nullptr;

		ModuleWindow* moduleWindow = nullptr;

		ModuleTexture* moduleTexture = nullptr;

		ModuleRenderer* moduleRenderer = nullptr;

		ModuleRegistry* moduleRegistry = nullptr;

		ModuleAnimation* moduleAnimation = nullptr;

		ModuleCollision* moduleCollision = nullptr;

		ModuleGameObject* moduleGameObject = nullptr;

		ModulePerformance* modulePerformance = nullptr;

		Module* gameModule = nullptr;

		Module* tmpGameModule = nullptr;
};

#endif