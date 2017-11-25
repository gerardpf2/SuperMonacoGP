#ifndef _MODULE_WINDOW_
#define _MODULE_WINDOW_

#include "Module.h"

struct SDL_Window;

class ModuleWindow : public Module
{
	public:

		ModuleWindow(GameEngine* gameEngine);

		virtual ~ModuleWindow();

		SDL_Window* getWindow() const;

		virtual bool setUp() override;

		virtual bool preUpdate(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		SDL_Window* window = nullptr;
};

#endif