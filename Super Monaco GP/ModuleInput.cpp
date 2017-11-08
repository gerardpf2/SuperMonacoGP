#include "ModuleInput.h"

ModuleInput::ModuleInput(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModuleInput::~ModuleInput()
{ }

bool ModuleInput::setUp()
{
	// Init SubSystem

	keyStates.resize(300, KeyState::IDLE); // 300 ¿?

	return true;
}

bool ModuleInput::preUpdate(float deltaTimeS)
{
	updateKeyStates();

	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		updateWindowState(event);
	}

	return true;
}

void ModuleInput::cleanUp()
{
	// Quit SubSystem

	keyStates.clear();
}

WindowState ModuleInput::getWindowState() const
{
	return windowState;
}

KeyState ModuleInput::getKeyState(unsigned int scancode) const
{
	return keyStates[scancode];
}

bool ModuleInput::getKeyPressed(unsigned int scancode) const
{
	KeyState keyState = getKeyState(scancode);
	return keyState == KeyState::DOWN || keyState == KeyState::REPEAT;
}

void ModuleInput::updateKeyStates()
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	for(unsigned int i = 0; i < (unsigned int)keyStates.size(); ++i)
	{
		if(keyboardState[i])
			keyStates[i] = getKeyState(i) == KeyState::IDLE ? KeyState::DOWN : KeyState::REPEAT;
		else
			keyStates[i] = getKeyPressed(i) ? KeyState::UP : KeyState::IDLE;
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