#ifndef _MODULE_RENDERER_
#define _MODULE_RENDERER_

#include "Module.h"
#include "Globals.h"

struct SDL_Rect;
struct SDL_Color;
struct SDL_Renderer;

class ModuleRenderer : public Module
{
	public:

		ModuleRenderer(GameEngine* gameEngine, bool active = true);

		virtual ~ModuleRenderer();

		virtual bool setUp() override;

		virtual bool preUpdate() override;

		virtual bool postUpdate() override;

		virtual void cleanUp() override;

		void renderLine(const Position2i& position0, const Position2i& position1, const SDL_Color& color) const;

		void renderRect(const SDL_Rect& rect, const SDL_Color& color) const;

	private:

		SDL_Renderer* renderer = nullptr;
};

#endif