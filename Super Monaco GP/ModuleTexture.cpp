#include "ModuleTexture.h"

#include <SDL.h>
#include "ModuleJson.h"
#include "GameEngine.h"
#include "ModuleRenderer.h"

using namespace std;
using namespace rapidjson;

ModuleTexture::ModuleTexture(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleTexture::~ModuleTexture()
{ }

uint ModuleTexture::load(const char* jsonPath)
{
	uint tmpTextureGroupId;
	if(isAlreadyLoaded(jsonPath, tmpTextureGroupId))
	{
		incrementLoadedCounter(tmpTextureGroupId, 1);
		return tmpTextureGroupId;
	}

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	// id, bmpPath, textures

	uint textureGroupId = jsonDocument["textureGroupId"].GetUint();
	const char* bmpPath = jsonDocument["bmpPath"].GetString();
	const Value& texturesJson = jsonDocument["textures"];

	SDL_Texture* texture = loadTexture(bmpPath);

	vector<Texture*>* textures = new vector<Texture*>();
	textures->reserve(texturesJson.Size());

	for(SizeType i = 0; i < texturesJson.Size(); ++i)
	{
		// textureId, rect, hFlipped

		uint textureId = i; // ¿?
		const Value& rectJson = texturesJson[i]["rect"];
		bool hFlipped = texturesJson[i]["hFlipped"].GetBool();

		SDL_Rect* rect = new SDL_Rect{ rectJson["x"].GetInt(), rectJson["y"].GetInt(), rectJson["w"].GetInt(), rectJson["h"].GetInt() };

		textures->push_back(new Texture{ texture, rect, hFlipped });
	}

	textureGroups[textureGroupId] = pair<SDL_Texture*, vector<Texture*>*>(texture, textures);

	loadedTextureGroups.push_back(pair<pair<string, uint>, uint>(pair<string, uint>(jsonPath, textureGroupId), 1));

	return textureGroupId;
}

void ModuleTexture::unload(uint idTextureGroup)
{
	string tmpJsonPath;
	if(isAlreadyUnloaded(idTextureGroup, tmpJsonPath)) return;

	incrementLoadedCounter(idTextureGroup, -1);
	if(getLoadedCounter(idTextureGroup) > 0) return;

	pair<SDL_Texture*, vector<Texture*>*>& textureGroup = textureGroups[idTextureGroup];

	unloadTexture(textureGroup.first);

	for(uint i = 0; i < textureGroup.second->size(); ++i)
	{
		Texture*& texture = (*textureGroup.second)[i];

		texture->t = nullptr;
		
		delete texture->r; texture->r = nullptr;

		delete texture; texture = nullptr;
	}

	textureGroup.second->clear();
	
	delete textureGroup.second;
	textureGroup.second = nullptr;

	textureGroups.erase(idTextureGroup);

	loadedTextureGroups.remove(pair<pair<string, uint>, uint>(pair<string, uint>(tmpJsonPath, idTextureGroup), 0));
}

const Texture* ModuleTexture::get(uint idTextureGroup, uint idTexture) const
{
	return (*textureGroups.at(idTextureGroup).second)[idTexture];
}

void ModuleTexture::cleanUp()
{
	textureGroups.clear();

	loadedTextureGroups.clear();
}

SDL_Texture* ModuleTexture::loadTexture(const char* bmpPath) const
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = SDL_LoadBMP(bmpPath);

	if(surface)
	{
		SDL_SetColorKey(surface, SDL_TRUE, 0xFF00FF);
		texture = SDL_CreateTextureFromSurface(getGameEngine()->getModuleRenderer()->getRenderer(), surface);

		SDL_FreeSurface(surface); surface = nullptr;
	}

	return texture;
}

void ModuleTexture::unloadTexture(SDL_Texture*& texture) const
{
	if(texture)
	{
		SDL_DestroyTexture(texture); texture = nullptr;
	}
}

bool ModuleTexture::isAlreadyLoaded(const string& jsonPath, uint& idTextureGroup) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedTextureGroups.begin(); it != loadedTextureGroups.end(); ++it)
		if(it->first.first == jsonPath)
		{
			idTextureGroup = it->first.second;

			return true;
		}

	return false;
}

bool ModuleTexture::isAlreadyUnloaded(uint idTextureGroup, string& jsonPath) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedTextureGroups.begin(); it != loadedTextureGroups.end(); ++it)
		if(it->first.second == idTextureGroup)
		{
			jsonPath = it->first.first;

			return false;
		}

	return true;
}

uint ModuleTexture::getLoadedCounter(uint idTextureGroup) const
{
	for(list<pair<pair<string, uint>, uint>>::const_iterator it = loadedTextureGroups.begin(); it != loadedTextureGroups.end(); ++it)
		if(it->first.second == idTextureGroup) return it->second;

	return 0;
}

void ModuleTexture::incrementLoadedCounter(uint idTextureGroup, int increment)
{
	for(list<pair<pair<string, uint>, uint>>::iterator it = loadedTextureGroups.begin(); it != loadedTextureGroups.end(); ++it)
		if(it->first.second == idTextureGroup)
		{
			it->second += increment;

			break;
		}
}