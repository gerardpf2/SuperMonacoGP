#ifndef _MODULE_START_
#define _MODULE_START_

#include <list>
#include <vector>
#include "Types.h"
#include "Module.h"
#include <SDL_rect.h>

struct CarStart;

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

		void checkSelectOption() const;

		void checkChangeOption();

		void render() const;

		void renderBase() const;

		void renderCars() const;

		void renderEnterPressed() const;

		void renderEnterNoPressed() const;

	private:

		uint audioGroupId = -1;

		uint textureGroupId = -1;

		const Texture* back = nullptr;

		const Texture* front = nullptr;

		const Texture* logo = nullptr;

		const Texture* options = nullptr;

		const Texture* arrow = nullptr;

		bool enterPressed = false;

		float pressEnterCounter = 0.0f;

		int selectedOption = 0;

		float arrowCounter = 0.0f;

		uint carIndex = 0;

		uint usedCars2Freq = 0;

		std::vector<CarStart*> cars;

		std::list<CarStart*> usedCars;

		// UI

		SDL_Rect backRect, frontRect, logoRect, optionsRect;

		SDL_Rect arrowRects[3];

		WindowPosition pressEnterPosition;

		WindowPosition option0Position, option1Position, option2Position;
};

#endif