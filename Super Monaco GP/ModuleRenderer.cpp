#include "ModuleRenderer.h"

#include "Globals.h"
#include "GameEngine.h"
#include "ModuleWindow.h"
#include <SDL2_gfxPrimitives.h>

using namespace std;

ModuleRenderer::ModuleRenderer(GameEngine* gameEngine) :
	Module(gameEngine)
{
	windowRect = SDL_Rect{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
}

ModuleRenderer::~ModuleRenderer()
{ }

SDL_Renderer* ModuleRenderer::getRenderer() const
{
	return renderer;
}

void ModuleRenderer::setLayer(uint id) const
{
	for(Layer* layer : layers)
		if(layer->id == id)
		{
			SDL_SetRenderTarget(renderer, layer->t);
			break;
		}
}

uint ModuleRenderer::addLayer(const SDL_Rect* textureRect, const SDL_Rect* viewport)
{
	uint id = layers.size();
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

	layers.push_back(new Layer{ id, texture, textureRect, viewport });

	return id;
}

void ModuleRenderer::removeLayer(uint id)
{
	for(Layer* layer : layers)
		if(layer->id == id)
		{
			SDL_DestroyTexture(layer->t);
			layer->t = nullptr;

			layer->v = nullptr;
			layer->tr = nullptr;

			layers.remove(layer);
			delete layer; layer = nullptr;

			break;
		}
}

void ModuleRenderer::setColorAtTop(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	colorAtTopR = r;
	colorAtTopG = g;
	colorAtTopB = b;
	colorAtTopA = a;

	renderColorAtTop = true;
}

void ModuleRenderer::renderRect(const SDL_Rect* dstRect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled) const
{
	if(filled) boxRGBA(renderer, dstRect->x, dstRect->y, dstRect->x + dstRect->w, dstRect->y + dstRect->h, r, g, b, a);
	else rectangleRGBA(renderer, dstRect->x, dstRect->y, dstRect->x + dstRect->w, dstRect->y + dstRect->h, r, g, b, a);
}

void ModuleRenderer::renderTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* dstRect, bool hFlipped, Uint8 modR, Uint8 modG, Uint8 modB, Uint8 modA) const
{
	SDL_SetTextureColorMod(texture, modR, modG, modB);
	SDL_SetTextureAlphaMod(texture, modA);

	SDL_RendererFlip flip = hFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	SDL_RenderCopyEx(renderer, texture, srcRect, dstRect, 0.0, nullptr, flip);
}

void ModuleRenderer::renderTrapezoid(const WindowTrapezoid& windowTrapezoid, uint color, bool filled) const
{
	short positionsX[4]{ windowTrapezoid.nl.x, windowTrapezoid.nr.x, windowTrapezoid.fr.x, windowTrapezoid.fl.x };
	short positionsY[4]{ windowTrapezoid.nl.y, windowTrapezoid.nr.y, windowTrapezoid.fr.y, windowTrapezoid.fl.y };

	// O.o ReportMemoryLeaks() o.O

	if(filled) filledPolygonColor(renderer, positionsX, positionsY, 4, color);
	else polygonColor(renderer, positionsX, positionsY, 4, color);
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

	for(Layer* layer : layers)
	{
		SDL_SetRenderTarget(renderer, layer->t);
		SDL_RenderClear(renderer);
	}

	SDL_SetRenderTarget(renderer, nullptr);

	renderColorAtTop = false;

	return true;
}

bool ModuleRenderer::postUpdate(float deltaTimeS)
{
	SDL_SetRenderTarget(renderer, nullptr);

	for(Layer* layer : layers)
		SDL_RenderCopy(renderer, layer->t, layer->tr, layer->v);

	if(renderColorAtTop)
		renderRect(&windowRect, colorAtTopR, colorAtTopG, colorAtTopB, colorAtTopA);

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

	layers.clear();
}