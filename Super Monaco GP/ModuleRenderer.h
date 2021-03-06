#ifndef _MODULE_RENDERER_
#define _MODULE_RENDERER_

#include <list>
#include <SDL.h>
#include "Types.h"
#include "Module.h"

class ModuleRenderer : public Module
{
	public:

		ModuleRenderer(GameEngine* gameEngine);

		virtual ~ModuleRenderer();

		SDL_Renderer* getRenderer() const;

		void setLayer(uint id) const;
		
		uint addLayer(const SDL_Rect* textureRect = nullptr, const SDL_Rect* viewport = nullptr);

		void removeLayer(uint id);

		void setColorAtTop(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

		void renderRect(const SDL_Rect* dstRect, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255, bool filled = true) const;

		void renderTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect, bool hFlipped = false, Uint8 modR = 255, Uint8 modG = 255, Uint8 modB = 255, Uint8 modA = 255) const;

		void renderTrapezoid(const WindowTrapezoid& windowTrapezoid, uint color, bool filled = true) const;

		virtual bool setUp() override;

		virtual bool preUpdate(float deltaTimeS) override;

		virtual bool postUpdate(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		std::list<Layer*> layers;

		SDL_Renderer* renderer = nullptr;

		SDL_Rect windowRect;

		bool renderColorAtTop = false;

		Uint8 colorAtTopR, colorAtTopG, colorAtTopB, colorAtTopA;
};

#endif