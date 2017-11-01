#ifndef _MODULE_RENDERER_
#define _MODULE_RENDERER_

#include "Module.h"

struct SDL_Renderer;

class ModuleRenderer : public Module
{
	public:

		ModuleRenderer(GameEngine* gameEngine, bool active = true);

		~ModuleRenderer();

		virtual bool setUp() override;

		virtual bool preUpdate() override;

		virtual bool postUpdate() override;

		virtual void cleanUp() override;

	private:

		SDL_Renderer* renderer = nullptr;
};

#endif