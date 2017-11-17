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

void ModuleRenderer::renderRectangle(const SDL_Rect& rect, unsigned int color, bool filled) const
{
	if(filled) boxColor(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
	else rectangleColor(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
}

void ModuleRenderer::renderLine(const Position2s& position0, const Position2s& position1, unsigned int color) const
{
	lineColor(renderer, position0.first, position0.second, position1.first, position1.second, color);
}

void ModuleRenderer::renderTrapezoid(const Position2s& position0, const Position2s& position1, const Position2s& position2, const Position2s& position3, unsigned int color, bool filled) const
{
	short positionsX[4]{ position0.first, position1.first, position2.first, position3.first };
	short positionsY[4]{ position0.second, position1.second, position2.second, position3.second };

	if(filled) filledPolygonColor(renderer, positionsX, positionsY, 4, color);
	else polygonColor(renderer, positionsX, positionsY, 4, color);
}