#include "ModuleInput.h"

#include <SDL.h>

ModuleInput::ModuleInput(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleInput::~ModuleInput()
{ }

WindowState ModuleInput::getWindowState() const
{
	return windowState;
}

bool ModuleInput::isKeyPressed(uint scancode) const
{
	KeyState keyState = getKeyState(scancode);
	
	return keyState == KeyState::DOWN || keyState == KeyState::REPEAT;
}

KeyState ModuleInput::getKeyState(uint scancode) const
{
	return keyStates[scancode];
}

bool ModuleInput::setUp()
{
	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		printf("ModuleInput::setUp -> ERROR: %s\n", SDL_GetError());

		return false;
	}

	keyStates.resize(300, KeyState::IDLE);

	return true;
}

bool ModuleInput::preUpdate(float deltaTimeS)
{
	updateKeyStates();

	SDL_Event event;

	while(SDL_PollEvent(&event))
		updateWindowState(event);

	return true;
}

void ModuleInput::cleanUp()
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	keyStates.clear();
}

void ModuleInput::updateKeyStates()
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	for(uint i = 0; i < (uint)keyStates.size(); ++i)
	{
		if(keyboardState[i])
			keyStates[i] = getKeyState(i) == KeyState::IDLE ? KeyState::DOWN : KeyState::REPEAT;
		else
			keyStates[i] = isKeyPressed(i) ? KeyState::UP : KeyState::IDLE;
	}
}

void ModuleInput::updateWindowState(const SDL_Event& event)
{
	switch(event.type)
	{
		case SDL_QUIT:
			windowState = WindowState::QUIT;

			break;
		case SDL_WINDOWEVENT:
			switch(event.window.event)
			{
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
				case SDL_WINDOWEVENT_FOCUS_LOST:
					windowState = WindowState::HIDE;

					break;
				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_RESTORED:
				case SDL_WINDOWEVENT_MAXIMIZED:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					windowState = WindowState::SHOW;

					break;
			}

			break;
	}
}