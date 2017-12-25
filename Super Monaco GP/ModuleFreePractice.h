#ifndef _MODULE_FREE_PRACTICE_
#define _MODULE_FREE_PRACTICE_

#include "ModuleWorld.h"
#include "ModuleFreePracticeUI.h"

class ModuleFreePractice : public ModuleWorld
{
	public:

		ModuleFreePractice(GameEngine* gameEngine);

		virtual ~ModuleFreePractice();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

		virtual void registerLapTime(const Car* car) const override;

	private:

		virtual void renderUI() const override;

		void updateFreePracticeCounter(float deltaTimeS);

	private:

		float freePracticeCounter = 0.0;

		const Texture* flag0 = nullptr;

		const Texture* flag1 = nullptr;

		WindowPosition freePracticePosition;

		SDL_Rect freePracticeFlag0Rect, freePracticeFlag1Rect;
};

#endif