#include "ModuleTexture.h"

#include <SDL.h>
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

const Texture* ModuleTexture::getTexture(TextureType textureType) const
{
	return textures[(uint)textureType];
}

bool ModuleTexture::setUp()
{
	Document texturesDocument;
	getGameEngine()->getModuleJson()->read("Resources/Configuration/Textures.json", texturesDocument);

	load(texturesDocument);

	return true;
}

void ModuleTexture::cleanUp()
{
	for(int i = (int)textureGroups.size() - 1; i >= 0; --i)
		unload(textureGroups[i]);

	textureGroups.clear();

	for(int i = (int)textures.size() - 1; i >= 0; --i)
	{
		textures[i]->t = nullptr;

		delete textures[i]->r;
		textures[i]->r = nullptr;

		delete textures[i];
		textures[i] = nullptr;
	}

	textures.clear();
}

void ModuleTexture::load(const Document& document)
{
	// TextureGroups
	// TextureGroup -> Type, Path

	const Value& textureGroupsJson  = document["textureGroups"];

	textureGroups.resize(textureGroupsJson.Size(), nullptr);

	for(SizeType i = 0; i < textureGroupsJson.Size(); ++i)
		load((TextureGroupType)textureGroupsJson[i]["type"].GetUint(), textureGroupsJson[i]["path"].GetString());

	// Textures
	// Texture -> Type, Group type, Rect

	const Value& texturesJson = document["textures"];

	textures.resize(texturesJson.Size(), nullptr);

	for(SizeType i = 0; i < texturesJson.Size(); ++i)
	{
		const Value& rJson = texturesJson[i]["rect"];

		SDL_Texture* t = textureGroups[texturesJson[i]["groupType"].GetUint()];
		SDL_Rect* r = new SDL_Rect{ rJson["x"].GetInt(), rJson["y"].GetInt(), rJson["w"].GetInt(), rJson["h"].GetInt() };

		Texture* texture = new Texture{ t, r };

		textures[texturesJson[i]["type"].GetUint()] = texture;
	}
}

void ModuleTexture::load(TextureGroupType textureGroupType, const char* texturePath)
{
	SDL_Surface* surface = SDL_LoadBMP(texturePath);

	if(surface)
	{
		SDL_SetColorKey(surface, SDL_TRUE, MAGENTA);

		textureGroups[(uint)textureGroupType] = SDL_CreateTextureFromSurface(getGameEngine()->getModuleRenderer()->getRenderer(), surface);

		SDL_FreeSurface(surface);
		surface = nullptr;
	}
}

void ModuleTexture::unload(SDL_Texture*& texture)
{
	if(texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
}