#ifndef _MODULE_ABOUT_
#define _MODULE_ABOUT_

#include "Types.h"
#include "Module.h"
#include <SDL_rect.h>

class ModuleAbout : public Module
{
	public:

		ModuleAbout(GameEngine* gameEngine);

		virtual ~ModuleAbout();

		virtual bool update(float deltaTimeS) override;

	private:

		void checkGoStart() const;

		void render() const;

		void renderInfo() const;

	private:

		SDL_Rect baseAllRect;

		WindowPosition aboutPosition;

		WindowPosition projectPosition, projectValuePosition;

		WindowPosition techPosition, techValuePosition;

		WindowPosition authorPosition, authorValuePosition;

		WindowPosition separationPosition;

		WindowPosition controlsPosition, controlsValuePosition0, controlsValuePosition1, controlsValuePosition2, controlsValuePosition3;
};

#endif