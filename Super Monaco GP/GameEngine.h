#ifndef _GAME_ENGINE_
#define _GAME_ENGINE_

#include <vector>
#include "Timer.h"

enum class GameModule
{
	SEGA,
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
class ModuleAudio;
class ModuleWindow;
class ModuleSwitch;
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

		Module* getGameModule() const;

		ModuleJson* getModuleJson() const;

		ModuleFont* getModuleFont() const;

		ModuleInput* getModuleInput() const;

		ModuleAudio* getModuleAudio() const;

		ModuleWindow* getModuleWindow() const;

		ModuleSwitch* getModuleSwitch() const;

		ModuleTexture* getModuleTexture() const;

		ModuleRenderer* getModuleRenderer() const;

		ModuleRegistry* getModuleRegistry() const;

		ModuleAnimation* getModuleAnimation() const;

		ModuleCollision* getModuleCollision() const;

		ModuleGameObject* getModuleGameObject() const;

		ModulePerformance* getModulePerformance() const;

		void setGameModule(GameModule gameModule, bool blocked = false);

		void run(GameModule gameModule = GameModule::START);

	private:

		void addInitialModules();

		bool setUp();

		bool mainLoop();

		bool setUpNewGameModule();

		void cleanUp();

	private:

		Timer timer;

		std::vector<Module*> modules;

		Module* gameModule = nullptr;

		Module* tmpGameModule = nullptr;

		ModuleJson* moduleJson = nullptr;

		ModuleFont* moduleFont = nullptr;

		ModuleInput* moduleInput = nullptr;

		ModuleAudio* moduleAudio = nullptr;

		ModuleWindow* moduleWindow = nullptr;

		ModuleSwitch* moduleSwitch = nullptr;

		ModuleTexture* moduleTexture = nullptr;

		ModuleRenderer* moduleRenderer = nullptr;

		ModuleRegistry* moduleRegistry = nullptr;

		ModuleAnimation* moduleAnimation = nullptr;

		ModuleCollision* moduleCollision = nullptr;

		ModuleGameObject* moduleGameObject = nullptr;

		ModulePerformance* modulePerformance = nullptr;
};

#endif