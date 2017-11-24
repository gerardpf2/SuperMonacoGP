#ifndef _MODULE_INPUT_
#define _MODULE_INPUT_

#include <SDL.h>
#include <vector>
#include "Types.h"
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

		virtual bool preUpdate(float deltaTimeS) override;

		virtual void cleanUp() override;

		WindowState getWindowState() const;

		KeyState getKeyState(uint scancode) const;

		bool getKeyPressed(uint scancode) const;

	private:

		void updateKeyStates();

		void updateWindowState(const SDL_Event& event);

	private:

		WindowState windowState;

		vector<KeyState> keyStates;
};

#endif