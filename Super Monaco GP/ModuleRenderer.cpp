#include "ModuleRenderer.h"

#include "GameEngine.h"
#include "ModuleWindow.h"
#include <SDL2_gfxPrimitives.h>

ModuleRenderer::ModuleRenderer(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModuleRenderer::~ModuleRenderer()
{ }

bool ModuleRenderer::setUp()
{
	renderer = SDL_CreateRenderer(getGameEngine()->getModuleWindow()->getWindow(), -1, SDL_RENDERER_PRESENTVSYNC);

	if(!renderer)
	{
		return false;
	}

	return true;
}

bool ModuleRenderer::preUpdate(float deltaTimeS)
{
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	return true;
}

bool ModuleRenderer::postUpdate(float deltaTimeS)
{
	SDL_RenderPresent(renderer);

	return true;
}

void ModuleRenderer::cleanUp()
{
	if(renderer)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
}

SDL_Renderer* ModuleRenderer::getRenderer() const
{
	return renderer;
}

/* void ModuleRenderer::renderRectangle(const SDL_Rect& rect, unsigned int color, bool filled) const
{
	if(filled) boxColor(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
	else rectangleColor(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
} */

/* void ModuleRenderer::renderLine(const ScreenPosition& position0, const ScreenPosition& position1, unsigned int color) const
{
	lineColor(renderer, position0.first, position0.second, position1.first, position1.second, color);
} */

void ModuleRenderer::renderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect) const
{
	SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void ModuleRenderer::renderTrapezoid(const ScreenPosition& position0, const ScreenPosition& position1, const ScreenPosition& position2, const ScreenPosition& position3, unsigned int color, bool filled) const
{
	short positionsX[4]{ position0.first, position1.first, position2.first, position3.first };
	short positionsY[4]{ position0.second, position1.second, position2.second, position3.second };

	if(filled) filledPolygonColor(renderer, positionsX, positionsY, 4, color);
	else polygonColor(renderer, positionsX, positionsY, 4, color);
}