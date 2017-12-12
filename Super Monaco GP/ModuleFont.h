#ifndef _MODULE_FONT_
#define _MODULE_FONT_

#include <vector>
#include "Types.h"
#include "Module.h"

class ModuleFont : public Module
{
	public:

		ModuleFont(GameEngine* gameEngine);

		virtual ~ModuleFont();

		void renderText(const std::string& text, const WindowPosition& position, float scale = 1.0f, bool leftToRight = true) const;

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		uint textureGroupId;

		std::vector<const Texture*> characters;
};

#endif