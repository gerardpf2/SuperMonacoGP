#ifndef _MODULE_TEXTURE_
#define _MODULE_TEXTURE_

#include <vector>
#include "Module.h"
#include "rapidjson/document.h"

enum class TextureGroupType
{
	PLAYER,
};

enum class TextureType
{
	PLAYER_TEXTURE_0,
	PLAYER_TEXTURE_1,
};

struct Texture;
struct SDL_Texture;

class ModuleTexture : public Module
{
	public:

		ModuleTexture(GameEngine* gameEngine);

		virtual ~ModuleTexture();

		const Texture* getTexture(TextureType textureType) const;

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		void load(const rapidjson::Document& document);

		void load(TextureGroupType textureGroupType, const char* texturePath);

		void unload(SDL_Texture*& texture);

	private:

		std::vector<Texture*> textures;

		std::vector<SDL_Texture*> textureGroups;
};

#endif