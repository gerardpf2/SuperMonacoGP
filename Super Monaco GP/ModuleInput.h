#ifndef _MODULE_INPUT_
#define _MODULE_INPUT_

#include <SDL.h>
#include <vector>
#include "Module.h"

using namespace std;

enum class WindowState
{
	QUIT,
	HIDE,
	SHOW,
};

enum class KeyState
{
	UP,
	DOWN,
	IDLE,
	REPEAT,
};

class ModuleInput : public Module
{
	public:

		ModuleInput(GameEngine* gameEngine, bool active = true);

		virtual ~ModuleInput();

		virtual bool setUp() override;

		virtual bool preUpdate() override;

		virtual void cleanUp() override;

		WindowState getWindowState() const;

		KeyState getKeyState(unsigned int scancode) const;

		bool getKeyPressed(unsigned int scancode) const;

	private:

		void updateKeyStates();

		void updateWindowState(const SDL_Event& event);

	private:

		WindowState windowState;

		vector<KeyState> keyStates;
};

#endif