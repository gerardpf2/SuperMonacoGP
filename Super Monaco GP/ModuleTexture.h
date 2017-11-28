#ifndef _MODULE_TEXTURE_
#define _MODULE_TEXTURE_

#include <map>
#include <vector>
#include "Types.h"
#include "Module.h"

struct Texture;
struct SDL_Texture;

class ModuleTexture : public Module
{
	public:

		ModuleTexture(GameEngine* gameEngine);

		virtual ~ModuleTexture();

		uint load(const char* jsonPath);

		void unload(uint idTextureGroup);

		const Texture* get(uint idTextureGroup, uint idTexture) const;

	private:

		SDL_Texture* loadTexture(const char* bmpPath) const;

		void unloadTexture(SDL_Texture*& texture) const;

	private:

		std::map<uint, std::pair<SDL_Texture*, std::vector<Texture*>*>> textureGroups;

		// std::map<const char*, uint> loadedTextureGroups;

	/* public:

		ModuleTexture(GameEngine* gameEngine);

		virtual ~ModuleTexture();

		const Texture* getTexture(uint textureId) const;

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		void load(const rapidjson::Document& documentJson);

		void loadTextureGroups(const rapidjson::Value& textureGroupsJson);

		void loadTextureGroup(const rapidjson::Value& textureGroupJson);

		void loadTextures(const rapidjson::Value& texturesJson);

		void loadTexture(const rapidjson::Value& textureJson);

		void loadTexture(uint textureGroupId, const char* texturePath);

		void unloadTexture(SDL_Texture*& texture);

	private:

		std::vector<Texture*> textures;

		std::vector<SDL_Texture*> textureGroups; */
};

#endif