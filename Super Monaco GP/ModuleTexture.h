#ifndef _MODULE_TEXTURE_
#define _MODULE_TEXTURE_

#include <vector>
#include "Module.h"

struct SDL_Texture;

enum class TextureType
{
	TEST,
};

class ModuleTexture : public Module
{
	public:

		ModuleTexture(GameEngine* gameEngine);

		virtual ~ModuleTexture();

		SDL_Texture* getTexture(TextureType textureType) const;

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		void load(const char* texturePath, TextureType textureType);

		void unload(SDL_Texture*& texture);

	private:

		std::vector<SDL_Texture*> textures;
};

#endif