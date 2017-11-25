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

		ModuleRenderer(GameEngine* gameEngine);

		virtual ~ModuleRenderer();

		SDL_Renderer* getRenderer() const;

		void renderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect) const;

		void renderTrapezoid(const WindowTrapezoid& windowTrapezoid, uint color) const;

		virtual bool setUp() override;

		virtual bool preUpdate(float deltaTimeS) override;

		virtual bool postUpdate(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		SDL_Renderer* renderer = nullptr;
};

#endif