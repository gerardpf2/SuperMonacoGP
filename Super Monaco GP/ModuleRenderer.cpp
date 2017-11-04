#include "ModuleRenderer.h"

#include <SDL.h>
#include "GameEngine.h"
#include "ModuleWindow.h"

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

bool ModuleRenderer::preUpdate()
{
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	return true;
}

bool ModuleRenderer::postUpdate()
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

void ModuleRenderer::renderLine(const Position2i& position0, const Position2i& position1, const SDL_Color& color) const
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, position0.x, position0.y, position1.x, position1.y);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void ModuleRenderer::renderRect(const SDL_Rect& rect, const SDL_Color& color) const
{
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}