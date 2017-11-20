#include "ModuleTexture.h"

#include <SDL.h>
#include "Colors.h"
#include "GameEngine.h"
#include "ModuleRenderer.h"

ModuleTexture::ModuleTexture(GameEngine* gameEngine, bool active) :
	Module(gameEngine, active)
{ }

ModuleTexture::~ModuleTexture()
{ }

bool ModuleTexture::setUp()
{
	textures.resize(1, nullptr);

	load("Resources/Textures/Test.bmp", TextureType::TEST);

	return true;
}

void ModuleTexture::cleanUp()
{
	for(int i = (int)textures.size() - 1; i >= 0; --i)
		unload(textures[i]);

	textures.clear();
}

SDL_Texture* ModuleTexture::getTexture(TextureType textureType) const
{
	return textures[(unsigned int)textureType];
}

void ModuleTexture::load(const char* texturePath, TextureType textureType)
{
	SDL_Surface* surface = SDL_LoadBMP(texturePath);

	if(surface)
	{
		SDL_SetColorKey(surface, SDL_TRUE, MAGENTA);

		textures[(unsigned int)textureType] = SDL_CreateTextureFromSurface(getGameEngine()->getModuleRenderer()->getRenderer(), surface);

		SDL_FreeSurface(surface);
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