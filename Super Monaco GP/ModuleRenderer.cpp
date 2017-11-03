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

// Test ---

#include "Camera.h"

float x = 0.0f;
float z = 10.0f;

Camera c(Position3f{ 0.0f, 10.0f, 0.0f }, 100.0f);

// --- Test

bool ModuleRenderer::postUpdate()
{
	// Test ---
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if(keys[SDL_SCANCODE_LEFT]) x -= 0.1f;
	if(keys[SDL_SCANCODE_RIGHT]) x += 0.1f;
	if(keys[SDL_SCANCODE_UP]) z += 0.1f;
	if(keys[SDL_SCANCODE_DOWN]) z -= 0.1f;

	if(keys[SDL_SCANCODE_A]) c.position.x -= 0.1f;
	if(keys[SDL_SCANCODE_D]) c.position.x += 0.1f;
	if(keys[SDL_SCANCODE_W]) c.position.y += 0.1f;
	if(keys[SDL_SCANCODE_S]) c.position.y -= 0.1f;

	Position2i p0, p1, p2, p3, p4, p5;

	c.getPositionWorldToScreen(Position3f{ x, 0.0f, z }, p0);
	c.getPositionWorldToScreen(Position3f{ x, 5.0f, z }, p1);

	c.getPositionWorldToScreen(Position3f{ -10.0f, 0.0f, 0.1f }, p2);
	c.getPositionWorldToScreen(Position3f{ -10.0f, 0.0f, 1000.0f }, p3);

	c.getPositionWorldToScreen(Position3f{ 10.0f, 0.0f, 0.1f }, p4);
	c.getPositionWorldToScreen(Position3f{ 10.0f, 0.0f, 1000.0f }, p5);

	SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
	SDL_RenderDrawLine(renderer, p4.x, p4.y, p5.x, p5.y);

	SDL_RenderDrawLine(renderer, 0, 240, 640, 240);
	SDL_RenderDrawLine(renderer, 320, 0, 320, 480);

	SDL_Rect r;
	r.x = p0.x - 5;
	r.y = p0.y - 5;
	r.w = 10;
	r.h = 10;

	SDL_RenderDrawRect(renderer, &r);

	SDL_Rect rr;
	rr.x = p1.x - 5;
	rr.y = p1.y - 5;
	rr.w = 10;
	rr.h = 10;

	SDL_RenderDrawRect(renderer, &rr);

	// --- Test

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