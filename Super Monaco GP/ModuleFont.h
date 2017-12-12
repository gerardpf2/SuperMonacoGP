#ifndef _MODULE_FONT_
#define _MODULE_FONT_

#include <SDL.h>
#include <vector>
#include "Types.h"
#include "Module.h"

enum class Alignment
{
	LEFT,
	RIGHT,
	CENTER,
};

class ModuleFont : public Module
{
	public:

		ModuleFont(GameEngine* gameEngine);

		virtual ~ModuleFont();

		void renderText(const std::string& text, const WindowPosition& position, Alignment alignment, float scale = 1.0f, Uint8 modR = 255, Uint8 modG = 255, Uint8 modB = 255) const;

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		float getTextWidth(const std::vector<const Texture*>& characterTextures, float scale) const;

		void getCharacterTextures(const std::string& text, std::vector<const Texture*>& characterTextures) const;

	private:

		uint textureGroupId;

		std::vector<const Texture*> characters;
};

#endif