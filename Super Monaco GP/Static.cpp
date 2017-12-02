#include "Static.h"

#include "Globals.h"
#include <SDL_rect.h>

Static::Static(uint id, const Texture* texture) :
	GameObject(id), texture(texture)
{
	size.w = (float)texture->r->w * SPRITE_SIZE_RATIO;
	size.h = (float)texture->r->h * SPRITE_SIZE_RATIO;
}

Static::~Static()
{ }

GameObjectType Static::getType() const
{
	return GameObjectType::STATIC;
}

const Texture* Static::getTexture() const
{
	return texture;
}

void Static::cleanUp()
{
	texture = nullptr;
}

const Texture* Static::getCurrentTexture() const
{
	return texture;
}