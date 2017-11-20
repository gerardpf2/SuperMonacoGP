#ifndef _MODULE_RENDERER_
#define _MODULE_RENDERER_

#include "Types.h"
#include "Module.h"

struct SDL_Rect;
struct SDL_Texture;
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

		SDL_Renderer* getRenderer() const;

		// void renderRectangle(const SDL_Rect& rect, unsigned int color, bool filled = false) const;

		// void renderLine(const ScreenPosition& position0, const ScreenPosition& position1, unsigned int color) const;

		void renderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect) const;

		void renderTrapezoid(const ScreenPosition& position0, const ScreenPosition& position1, const ScreenPosition& position2, const ScreenPosition& position3, unsigned int color, bool filled = false) const;

	private:

		SDL_Renderer* renderer = nullptr;
};

#endif