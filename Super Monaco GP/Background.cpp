#include "Background.h"

#include "Road.h"
#include "Camera.h"
#include "Globals.h"
#include "Segment.h"
#include <SDL_rect.h>
#include "ModuleRenderer.h"

Background::Background(const Texture* texture, const Texture* textureSky, const Road* road) :
	texture(texture), textureSky(textureSky), road(road)
{
	textureRect = SDL_Rect{ texture->r->x, texture->r->y, 256, 72 };
	renderRect = SDL_Rect{ 0, 0, WINDOW_WIDTH, (int)(72.0f * WINDOW_WIDTH / 256.0f) };
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

const Road* Background::getRoad() const
{
	return road;
}

float Background::getOffsetX() const
{
	return offsetX;
}

void Background::update(const Camera* camera, float deltaTimeS)
{
	
}

void Background::render(const Camera* camera, const ModuleRenderer* moduleRenderer)
{
	// Project the last position that can be seen to determine where the background needs to be drawn
	// This guarantees its correct positioning no matter the camera y, camera fov or camera mirror

	float z = camera->getPosition()->z + (camera->getForward() ? 1.0f : -1.0f) * DRAW_DISTANCE;
	// float y = 0.0f;
	float y = road->getSegmentAtZ(z)->getYNear();
	// float y = road->getSegmentAtZ(camera->getBasePositionZ())->getYNear();

	WorldPosition worldPosition{ 0.0f, y, z };
	WindowPosition windowPosition;

	camera->project(worldPosition, windowPosition);

	renderRect.y = windowPosition.y - renderRect.h;

	// Update offsetX

	// Render Sky

	moduleRenderer->renderTexture(textureSky->t, textureSky->r, nullptr);

	// Render

	moduleRenderer->renderTexture(texture->t, &textureRect, &renderRect);
}