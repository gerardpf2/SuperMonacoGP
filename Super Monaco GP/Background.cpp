#include "Background.h"

#include "Road.h"
#include "Utils.h"
#include "Player.h"
#include "Segment.h"
#include "ModuleRenderer.h"

Background::Background(const Texture* texture, const Texture* textureSky, const Texture* textureGround, float projectionY1) :
	texture(texture), textureSky(textureSky), textureGround(textureGround)
{
	assert(texture);
	assert(texture->r);

	textureRect = SDL_Rect{ texture->r->x, texture->r->y, BACKGROUND_REGION_X, BACKGROUND_REGION_Y };

	float hTexture = BACKGROUND_REGION_Y * WINDOW_WIDTH / BACKGROUND_REGION_X;
	float yTexture = WINDOW_HEIGHT - WINDOW_HEIGHT / projectionY1 - hTexture;

	float yTextureSky = 0.0f;
	float hTextureSky = yTexture;
	
	float yTextureGround = yTexture + hTexture;
	float hTextureGround = WINDOW_HEIGHT - yTextureGround;

	renderTextureRect = SDL_Rect{ 0, (int)yTexture, WINDOW_WIDTH, (int)hTexture };
	renderTextureSkyRect = SDL_Rect{ 0, (int)yTextureSky, WINDOW_WIDTH, (int)hTextureSky };
	renderTextureGroundRect = SDL_Rect{ 0, (int)yTextureGround, WINDOW_WIDTH, (int)hTextureGround };
}

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

const Texture* Background::getTextureGround() const
{
	return textureGround;
}

float Background::getOffsetX() const
{
	return offsetX;
}

float Background::getOffsetY() const
{
	return offsetY;
}

void Background::update(const Player* player, const Road* road, float deltaTimeS)
{
	assert(road);
	assert(player);
	assert(texture);
	assert(texture->r);
	assert(player->getPosition());

	Segment* playerSegment = road->getSegmentAtZ(player->getPosition()->z);

	assert(playerSegment);

	offsetX += BACKGROUND_VELOCITY_OFFSET_X * texture->r->w * playerSegment->getCurve() * player->getVelocityPercent() * deltaTimeS;
	offsetX = mod0L(offsetX, (float)texture->r->w);

	float incY = playerSegment->getYFar() - playerSegment->getYNear();
	offsetY += BACKGROUND_VELOCITY_OFFSET_Y * texture->r->h * incY * player->getVelocityPercent() * deltaTimeS;
}

void Background::render(bool mirror, const ModuleRenderer* moduleRenderer) const
{
	assert(texture);
	assert(textureSky);
	assert(textureGround);
	assert(moduleRenderer);

	SDL_Rect renderRectSky = renderTextureSkyRect;
	SDL_Rect renderRectGround = renderTextureGroundRect;
	SDL_Rect renderRect = renderTextureRect;

	renderRectSky.h += (int)offsetY;
	renderRectGround.y += (int)offsetY;
	renderRectGround.h -= (int)offsetY;
	renderRect.y += (int)offsetY;

	// Render Sky

	moduleRenderer->renderTexture(textureSky->t, textureSky->r, &renderRectSky, textureSky->hFlipped);

	// Render Ground

	moduleRenderer->renderTexture(textureGround->t, textureGround->r, &renderRectGround, textureGround->hFlipped);

	// Render

	SDL_Rect rect0, rect1, renderRect0, renderRect1;

	if(!getRectsEndlessTexture(texture, textureRect, renderRect, mirror, offsetX, rect0, renderRect0, rect1, renderRect1))
		moduleRenderer->renderTexture(texture->t, &rect0, &renderRect0, texture->hFlipped || mirror);
	else
	{
		moduleRenderer->renderTexture(texture->t, &rect0, &renderRect0, texture->hFlipped || mirror);
		moduleRenderer->renderTexture(texture->t, &rect1, &renderRect1, texture->hFlipped || mirror);
	}
}