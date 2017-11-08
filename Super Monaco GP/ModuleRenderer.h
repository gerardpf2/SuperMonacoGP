#ifndef _MODULE_RENDERER_
#define _MODULE_RENDERER_

#include "Module.h"
#include "Globals.h"

struct SDL_Rect;
struct SDL_Renderer;

class ModuleRenderer : public Module
{
	public:

		ModuleRenderer(GameEngine* gameEngine, bool active = true);

		virtual ~ModuleRenderer();

		virtual bool setUp() override;

		virtual bool preUpdate(float deltaTimeS) override;

		virtual bool postUpdate(float deltaTimeS) override;

		virtual void cleanUp() override;

		void renderRectangle(const SDL_Rect& rect, unsigned int color, bool filled = false) const;

		void renderLine(const Position2s& position0, const Position2s& position1, unsigned int color) const;

		void renderTrapezoid(const Position2s& position0, const Position2s& position1, const Position2s& position2, const Position2s& position3, unsigned int color, bool filled = false) const;

	private:

		SDL_Renderer* renderer = nullptr;
};

#endif