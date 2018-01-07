#ifndef _MODULE_TEXTURE_
#define _MODULE_TEXTURE_

#include <map>
#include <list>
#include <vector>
#include "Types.h"
#include "Module.h"

class ModuleTexture : public Module
{
	public:

		ModuleTexture(GameEngine* gameEngine);

		virtual ~ModuleTexture();

		uint load(const char* jsonPath);

		void unload(uint idTextureGroup);

		const Texture* get(uint idTextureGroup, uint idTexture) const;

		virtual void cleanUp() override;

	private:

		SDL_Texture* loadTexture(const char* bmpPath) const;

		void unloadTexture(SDL_Texture*& texture) const;

		bool isAlreadyLoaded(const std::string& jsonPath, uint& idTextureGroup) const;

		bool isAlreadyUnloaded(uint idTextureGroup, std::string& jsonPath) const;

		uint getLoadedCounter(uint idTextureGroup) const;

		void incrementLoadedCounter(uint idTextureGroup, int increment);

	private:

		/*
		
		For each texture group
			Vector of "big" textures (each one of these defines a textuure group)
			Vector of specific textures (pointer to its corresponding "big" texture and the definition of its rect)
		
		*/

		std::map<uint, std::pair<SDL_Texture*, std::vector<Texture*>*>> textureGroups;

		/*
		
		For each used texture group
			Path to its json file and its corresponding texture group id
			Times loaded counter
		
		*/

		std::list<std::pair<std::pair<std::string, uint>, uint>> loadedTextureGroups;
};

#endif