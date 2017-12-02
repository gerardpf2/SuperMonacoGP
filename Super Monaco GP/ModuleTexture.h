#ifndef _MODULE_TEXTURE_
#define _MODULE_TEXTURE_

#include <map>
#include <list>
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

		bool isAlreadyLoaded(const std::string& jsonPath, uint& idTextureGroup) const;

		bool isAlreadyUnloaded(uint idTextureGroup, std::string& jsonPath) const;

	private:

		std::map<uint, std::pair<SDL_Texture*, std::vector<Texture*>*>> textureGroups;

		std::list<std::pair<std::string, uint>> loadedTextureGroups;
};

#endif