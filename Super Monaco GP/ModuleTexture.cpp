#include "ModuleTexture.h"

#include <SDL.h>
#include "Colors.h"
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
	// Check if already loaded

	// map<const char*, uint>::iterator it = loadedTextureGroups.find(jsonPath);
	// if(it != loadedTextureGroups.end()) return it->second;

	Document jsonDocument;
	getGameEngine()->getModuleJson()->read(jsonPath, jsonDocument);

	// id, bmpPath, textures

	uint textureGroupId = jsonDocument["textureGroupId"].GetUint();
	const char* bmpPath = jsonDocument["bmpPath"].GetString();
	const Value& texturesJson = jsonDocument["textures"];

	unload(textureGroupId);

	SDL_Texture* texture = loadTexture(bmpPath);

	vector<Texture*>* textures = new vector<Texture*>(); // texturesJson.Size(), nullptr);
	textures->reserve(texturesJson.Size());

	for(SizeType i = 0; i < texturesJson.Size(); ++i)
	{
		// textureId, rect

		uint textureId = i; // texturesJson[i]["textureId"].GetUint();
		const Value& rectJson = texturesJson[i]; // texturesJson[i]["rect"];

		SDL_Rect* rect = new SDL_Rect{ rectJson["x"].GetInt(), rectJson["y"].GetInt(), rectJson["w"].GetInt(), rectJson["h"].GetInt() };

		// (*textures)[textureId] = new Texture{ texture, rect };
		textures->push_back(new Texture{ texture, rect });
	}

	textureGroups[textureGroupId] = pair<SDL_Texture*, vector<Texture*>*>(texture, textures);

	// loadedTextureGroups[jsonPath] = textureGroupId;

	return textureGroupId;
}

void ModuleTexture::unload(uint idTextureGroup)
{
	// Check if already unloaded

	if(textureGroups.find(idTextureGroup) == textureGroups.end()) return;

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
}

const Texture* ModuleTexture::get(uint idTextureGroup, uint idTexture) const
{
	return (*textureGroups.at(idTextureGroup).second)[idTexture];
}

SDL_Texture* ModuleTexture::loadTexture(const char* bmpPath) const
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = SDL_LoadBMP(bmpPath);

	if(surface)
	{
		SDL_SetColorKey(surface, SDL_TRUE, MAGENTA);
		
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

/* #include <SDL.h>
#include "Colors.h"
#include "GameEngine.h"
#include "ModuleJson.h"
#include "ModuleRenderer.h"

using namespace rapidjson;

ModuleTexture::ModuleTexture(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleTexture::~ModuleTexture()
{ }

const Texture* ModuleTexture::getTexture(uint textureId) const
{
	return textures[textureId];
}

bool ModuleTexture::setUp()
{
	Document documentJson;
	getGameEngine()->getModuleJson()->read("Resources/Configuration/Textures.json", documentJson);

	load(documentJson);

	return true;
}

void ModuleTexture::cleanUp()
{
	for(int i = (int)textureGroups.size() - 1; i >= 0; --i)
		unloadTexture(textureGroups[i]);

	textureGroups.clear();

	for(int i = (int)textures.size() - 1; i >= 0; --i)
	{
		if(textures[i])
		{
			textures[i]->t = nullptr;

			delete textures[i]->r;
			textures[i]->r = nullptr;

			delete textures[i];
			textures[i] = nullptr;
		}
	}

	textures.clear();
}

void ModuleTexture::load(const Document& documentJson)
{
	// Texture groups must be loaded first

	loadTextureGroups(documentJson["textureGroups"]);
	loadTextures(documentJson["textures"]);
}

void ModuleTexture::loadTextureGroups(const Value& textureGroupsJson)
{
	textureGroups.resize(textureGroupsJson.Size(), nullptr);

	for(SizeType i = 0; i < textureGroupsJson.Size(); ++i)
		loadTextureGroup(textureGroupsJson[i]);
}

void ModuleTexture::loadTextureGroup(const Value& textureGroupJson)
{
	// Id, Path

	loadTexture(textureGroupJson["id"].GetUint(), textureGroupJson["path"].GetString());
}

void ModuleTexture::loadTextures(const Value& texturesJson)
{
	textures.resize(texturesJson.Size(), nullptr);

	for(SizeType i = 0; i < texturesJson.Size(); ++i)
		loadTexture(texturesJson[i]);
}

void ModuleTexture::loadTexture(const Value& textureJson)
{
	// Id, Group id, Rect

	const Value& rJson = textureJson["rect"];

	SDL_Texture* t = textureGroups[textureJson["groupId"].GetUint()];
	SDL_Rect* r = new SDL_Rect{ rJson["x"].GetInt(), rJson["y"].GetInt(), rJson["w"].GetInt(), rJson["h"].GetInt() };

	textures[textureJson["id"].GetUint()] = new Texture{ t, r };
}

void ModuleTexture::loadTexture(uint textureGroupId, const char* texturePath)
{
	SDL_Surface* surface = SDL_LoadBMP(texturePath);

	if(surface)
	{
		SDL_SetColorKey(surface, SDL_TRUE, MAGENTA);

		textureGroups[textureGroupId] = SDL_CreateTextureFromSurface(getGameEngine()->getModuleRenderer()->getRenderer(), surface);

		SDL_FreeSurface(surface);
		surface = nullptr;
	}
}

void ModuleTexture::unloadTexture(SDL_Texture*& texture)
{
	if(texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
} */