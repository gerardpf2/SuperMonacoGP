#ifndef _MODULE_ABOUT_
#define _MODULE_ABOUT_

#include "Types.h"
#include "Module.h"
#include <SDL_rect.h>

class Animation;

class ModuleAbout : public Module
{
	public:

		ModuleAbout(GameEngine* gameEngine);

		virtual ~ModuleAbout();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		void checkGoStart() const;

		void updateCar(float deltaTimeS);

		void render() const;

		void renderCar() const;

		void renderInfo() const;

	private:

		uint carAnimationGroupId = -1;

		Animation* carAnimation = nullptr;

		SDL_Rect baseAllRect, carRect;

		WindowPosition aboutPosition;

		WindowPosition projectPosition, projectValuePosition;

		WindowPosition techPosition, techValuePosition;

		WindowPosition authorPosition, authorValuePosition;

		WindowPosition separationPosition;

		WindowPosition controlsPosition, controlsValuePosition0, controlsValuePosition1, controlsValuePosition2, controlsValuePosition3;
};

#endif