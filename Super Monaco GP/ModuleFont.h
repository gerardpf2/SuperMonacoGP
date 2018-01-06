#ifndef _MODULE_FONT_
#define _MODULE_FONT_

#include <SDL.h>
#include <vector>
#include "Types.h"
#include "Module.h"

enum class HAlignment
{
	LEFT,
	RIGHT,
	CENTER,
};

enum class VAlignment
{
	TOP,
	BOTTOM,
	CENTER,
};

class ModuleFont : public Module
{
	public:

		ModuleFont(GameEngine* gameEngine);

		virtual ~ModuleFont();

		void renderText(const std::string& text, const WindowPosition& position, HAlignment hAlignment, VAlignment vAlignment, float scaleW = 1.0f, float scaleH = 1.0f, Uint8 modR = 255, Uint8 modG = 255, Uint8 modB = 255, Uint8 modA = 255) const;

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		void getTextWidthHeight(const std::vector<const Texture*>& characterTextures, float scaleW, float scaleH, float& width, float& height) const;

		void getCharacterTextures(const std::string& text, std::vector<const Texture*>& characterTextures) const;

	private:

		uint textureGroupId = -1;

		std::vector<const Texture*> characters;
};

#endif