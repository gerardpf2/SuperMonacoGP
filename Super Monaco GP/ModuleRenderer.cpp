#include "ModuleRenderer.h"

#include "GameEngine.h"
#include "ModuleWindow.h"
#include <SDL2_gfxPrimitives.h>

ModuleRenderer::ModuleRenderer(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleRenderer::~ModuleRenderer()
{ }

SDL_Renderer* ModuleRenderer::getRenderer() const
{
	return renderer;
}

void ModuleRenderer::renderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect) const
{
	SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void ModuleRenderer::renderTrapezoid(const WindowTrapezoid& windowTrapezoid, uint color) const
{
	short positionsX[4]{ windowTrapezoid.nl.x, windowTrapezoid.nr.x, windowTrapezoid.fr.x, windowTrapezoid.fl.x };
	short positionsY[4]{ windowTrapezoid.nl.y, windowTrapezoid.nr.y, windowTrapezoid.fr.y, windowTrapezoid.fl.y };

	// O.o ReportMemoryLeaks() o.O
	filledPolygonColor(renderer, positionsX, positionsY, 4, color);
}

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