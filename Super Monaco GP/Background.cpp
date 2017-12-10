#include "Background.h"

#include "Road.h"
#include "Utils.h"
#include "Types.h"
#include "Camera.h"
#include "Player.h"
#include "Globals.h"
#include "Segment.h"
#include <SDL_rect.h>
#include "ModuleRenderer.h"

Background::Background(const Texture* texture, const Texture* textureSky, const Texture* textureGround, float projectionY1) :
	texture(texture), textureSky(textureSky), textureGround(textureGround)
{
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
	Segment* playerSegment = road->getSegmentAtZ(player->getPosition()->z);

	offsetX += BACKGROUND_VELOCITY_OFFSET_X * texture->r->w * playerSegment->getCurve() * player->getVelocityPercent() * deltaTimeS;
	offsetX = mod0L(offsetX, (float)texture->r->w);

	float incY = playerSegment->getYFar() - playerSegment->getYNear();
	offsetY += BACKGROUND_VELOCITY_OFFSET_Y * texture->r->h * incY * player->getVelocityPercent() * deltaTimeS;
}

void Background::render(bool mirror, const ModuleRenderer* moduleRenderer) const
{
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

	SDL_Rect rect0 = textureRect;
	rect0.x += (!mirror ? (int)offsetX : (int)mod0L(offsetX + texture->r->w / 2.0f, (float)texture->r->w));

	if(rect0.x + rect0.w <= texture->r->x + texture->r->w)
		moduleRenderer->renderTexture(texture->t, &rect0, &renderRect, texture->hFlipped || mirror);
	else
	{
		rect0.w -= rect0.x + rect0.w - (texture->r->x + texture->r->w);

		SDL_Rect rect1 = textureRect;

		if(!mirror)
		{
			rect1.x = textureRect.x;
			rect1.w = textureRect.w - rect0.w;
		}
		else
		{
			rect1.x = rect0.x;
			rect1.w = rect0.w;

			rect0.x = textureRect.x;
			rect0.w = textureRect.w - rect1.w;
		}

		SDL_Rect renderRect0 = renderRect;
		renderRect0.w = (int)(renderRect0.w * ((float)rect0.w / textureRect.w));

		SDL_Rect renderRect1 = renderRect;
		renderRect1.x = renderRect0.w;
		renderRect1.w = renderRect.w - renderRect0.w;

		moduleRenderer->renderTexture(texture->t, &rect0, &renderRect0, texture->hFlipped || mirror);
		moduleRenderer->renderTexture(texture->t, &rect1, &renderRect1, texture->hFlipped || mirror);
	}
}