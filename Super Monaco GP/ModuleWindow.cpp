#include "ModuleWindow.h"

#include <SDL.h>
#include "Globals.h"
#include "GameEngine.h"
#include "ModuleInput.h"

ModuleWindow::ModuleWindow(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleWindow::~ModuleWindow()
{ }

SDL_Window* ModuleWindow::getWindow() const
{
	return window;
}

bool ModuleWindow::setUp()
{
	if(SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

		if(!window)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool ModuleWindow::preUpdate(float deltaTimeS)
{
	if(getGameEngine()->getModuleInput()->isKeyPressed(SDL_SCANCODE_ESCAPE)) return false;
	if(getGameEngine()->getModuleInput()->getWindowState() == WindowState::QUIT) return false;

	return true;
}

void ModuleWindow::cleanUp()
{
	if(window)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_Quit();
}