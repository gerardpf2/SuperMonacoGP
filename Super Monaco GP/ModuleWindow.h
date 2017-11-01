#ifndef _MODULE_WINDOW_
#define _MODULE_WINDOW_

#include "Module.h"

struct SDL_Window;

class ModuleWindow : public Module
{
	public:

		ModuleWindow(GameEngine* gameEngine, bool active = true);

		virtual ~ModuleWindow();

		virtual bool setUp() override;

		virtual bool preUpdate() override;

		virtual void cleanUp() override;

		SDL_Window* getWindow() const;

	private:

		SDL_Window* window = nullptr;
};

#endif