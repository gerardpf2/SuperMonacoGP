#include "Static.h"

#include <assert.h>
#include "Globals.h"
#include <SDL_rect.h>

Static::Static(const Texture* texture) :
	texture(texture)
{
	assert(texture);
	assert(texture->r);

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

const Texture* Static::getCurrentTexture(bool mirror) const
{
	return texture;
}