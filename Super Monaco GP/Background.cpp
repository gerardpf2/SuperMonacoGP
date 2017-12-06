#include "Background.h"

#include "Camera.h"
#include <SDL_rect.h>
#include "ModuleRenderer.h"

Background::Background(const Texture* texture, const Texture* textureSky) :
	texture(texture), textureSky(textureSky)
{ }

Background::~Background()
{ }

const Texture* Background::getTexture() const
{
	return texture;
}

const Texture* Background::getTextureSky() const
{
	return textureSky;
}

float Background::getOffsetX() const
{
	return offsetX;
}

void Background::render(const Camera* camera, const ModuleRenderer* moduleRenderer)
{
	// Update offsetX

	// Render Sky

	moduleRenderer->renderTexture(textureSky->t, textureSky->r, nullptr);

	// Render
}