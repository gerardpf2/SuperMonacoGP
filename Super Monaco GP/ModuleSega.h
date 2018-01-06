#ifndef _MODULE_SEGA_
#define _MODULE_SEGA_

#include "Types.h"
#include "Module.h"
#include <SDL_rect.h>

class Animation;

class ModuleSega : public Module
{
	public:

		ModuleSega(GameEngine* gameEngine);

		virtual ~ModuleSega();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		void updateLogo(float deltaTimeS) const;

		void checkGoStart() const;

		void render() const;

	private:

		uint logoAnimationGroupId = -1;

		Animation* logoAnimation = nullptr;

		// UI

		SDL_Rect baseAllRect, logoRect;
};

#endif