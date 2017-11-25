#ifndef _MODULE_INPUT_
#define _MODULE_INPUT_

#include <vector>
#include "Types.h"
#include "Module.h"
#include <SDL_events.h>

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

		ModuleInput(GameEngine* gameEngine);

		virtual ~ModuleInput();

		WindowState getWindowState() const;

		bool isKeyPressed(uint scancode) const;

		KeyState getKeyState(uint scancode) const;

		virtual bool setUp() override;

		virtual bool preUpdate(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		void updateKeyStates();

		void updateWindowState(const SDL_Event& event);

	private:

		WindowState windowState;

		std::vector<KeyState> keyStates;
};

#endif