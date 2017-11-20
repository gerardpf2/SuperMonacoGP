#ifndef _MODULE_TEXTURE_
#define _MODULE_TEXTURE_

#include <vector>
#include "Module.h"

using namespace std;

struct SDL_Texture;

enum class TextureType
{
	TEST, //
};

class ModuleTexture : public Module
{
	public:

		ModuleTexture(GameEngine* gameEngine, bool active = true);

		virtual ~ModuleTexture();

		virtual bool setUp() override;

		virtual void cleanUp() override;

		SDL_Texture* getTexture(TextureType textureType) const;

	private:

		void load(const char* texturePath, TextureType textureType);

		void unload(SDL_Texture*& texture);


	private:

		vector<SDL_Texture*> textures;
};

#endif