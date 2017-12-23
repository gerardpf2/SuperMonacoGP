#ifndef _MODULE_START_
#define _MODULE_START_

#include "Types.h"
#include "Module.h"
#include "Globals.h"
#include <SDL_rect.h>

// Window coordinates ---

#define BACK_RECT_X 0
#define BACK_RECT_Y 0
#define BACK_RECT_W WINDOW_WIDTH
#define BACK_RECT_H WINDOW_HEIGHT
#define BACK_RECT SDL_Rect{ BACK_RECT_X, BACK_RECT_Y, BACK_RECT_W, BACK_RECT_H }

#define FRONT_RECT_X 0
#define FRONT_RECT_Y 0
#define FRONT_RECT_W WINDOW_WIDTH
#define FRONT_RECT_H WINDOW_HEIGHT
#define FRONT_RECT SDL_Rect{ FRONT_RECT_X, FRONT_RECT_Y, FRONT_RECT_W, FRONT_RECT_H }

#define LOGO_RECT_X (int)(0.55f * WINDOW_WIDTH)
#define LOGO_RECT_Y (int)(0.85f * WINDOW_HEIGHT)
#define LOGO_RECT_W (int)(240 * WINDOW_SCALE)
#define LOGO_RECT_H (int)(16 * WINDOW_SCALE)
#define LOGO_RECT SDL_Rect{ LOGO_RECT_X, LOGO_RECT_Y, LOGO_RECT_W, LOGO_RECT_H }

#define PRESS_ENTER_POSITION_SCALE WINDOW_SCALE
#define PRESS_ENTER_POSITION_X (int)(0.5f * WINDOW_WIDTH)
#define PRESS_ENTER_POSITION_Y (int)(0.5f * WINDOW_HEIGHT)
#define PRESS_ENTER_POSITION WindowPosition{ PRESS_ENTER_POSITION_X, PRESS_ENTER_POSITION_Y }

#define OPTIONS_RECT_X (int)(0.375f * WINDOW_WIDTH)
#define OPTIONS_RECT_Y (int)(0.572f * WINDOW_HEIGHT)
#define OPTIONS_RECT_W (int)(368 * WINDOW_SCALE)
#define OPTIONS_RECT_H (int)(144 * WINDOW_SCALE)
#define OPTIONS_RECT SDL_Rect{ OPTIONS_RECT_X, OPTIONS_RECT_Y, OPTIONS_RECT_W, OPTIONS_RECT_H }

#define OPTION_POSITION_SCALE_W (1.25f * WINDOW_SCALE)
#define OPTION_POSITION_SCALE_H (0.75f * WINDOW_SCALE)
#define OPTION_POSITION_X (int)(OPTIONS_RECT_X + 0.125f * OPTIONS_RECT_W)
#define OPTION_0_POSITION_Y (int)(OPTIONS_RECT_Y + 0.25f * OPTIONS_RECT_H)
#define OPTION_0_POSITION WindowPosition{ OPTION_POSITION_X, OPTION_0_POSITION_Y }
#define OPTION_1_POSITION_Y (int)(OPTIONS_RECT_Y + 0.5f * OPTIONS_RECT_H)
#define OPTION_1_POSITION WindowPosition{ OPTION_POSITION_X, OPTION_1_POSITION_Y }
#define OPTION_2_POSITION_Y (int)(OPTIONS_RECT_Y + 0.75f * OPTIONS_RECT_H)
#define OPTION_2_POSITION WindowPosition{ OPTION_POSITION_X, OPTION_2_POSITION_Y }

#define ARROW_RECT_X (int)(OPTIONS_RECT_X + 0.075f * OPTIONS_RECT_W)
#define ARROW_RECT_W (int)(14 * WINDOW_SCALE)
#define ARROW_RECT_H (int)(12 * WINDOW_SCALE)
#define ARROW_0_RECT_Y (int)(OPTIONS_RECT_Y + 0.2f * OPTIONS_RECT_H)
#define ARROW_0_RECT SDL_Rect{ ARROW_RECT_X, ARROW_0_RECT_Y, ARROW_RECT_W, ARROW_RECT_H }
#define ARROW_1_RECT_Y (int)(OPTIONS_RECT_Y + 0.45f * OPTIONS_RECT_H)
#define ARROW_1_RECT SDL_Rect{ ARROW_RECT_X, ARROW_1_RECT_Y, ARROW_RECT_W, ARROW_RECT_H }
#define ARROW_2_RECT_Y (int)(OPTIONS_RECT_Y + 0.7f * OPTIONS_RECT_H)
#define ARROW_2_RECT SDL_Rect{ ARROW_RECT_X, ARROW_2_RECT_Y, ARROW_RECT_W, ARROW_RECT_H }

// --- Window coordinates

struct Texture;

class ModuleStart : public Module
{
	public:

		ModuleStart(GameEngine* gameEngine);

		virtual ~ModuleStart();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		void updateCars(float deltaTimeS);

		void updateEnterPressed(float deltaTimeS);

		void updateEnterNoPressed(float deltaTimeS);

		void selectOption() const;

		void renderBase() const;

		void renderCars() const;

		void renderEnterPressed() const;

		void renderEnterNoPressed() const;

	private:

		uint textureGroupId = -1;

		const Texture* back = nullptr;

		const Texture* front = nullptr;

		const Texture* logo = nullptr;

		const Texture* options = nullptr;

		const Texture* arrow = nullptr;

		bool enterPressed = false;

		float pressEnterCounter = 0.0;

		int selectedOption = 0;

		float arrowCounter = 0.0;

		SDL_Rect backRect, frontRect, logoRect, optionsRect;

		SDL_Rect arrowRects[3];

		WindowPosition pressEnterPosition;

		WindowPosition option0Position, option1Position, option2Position;
};

#endif