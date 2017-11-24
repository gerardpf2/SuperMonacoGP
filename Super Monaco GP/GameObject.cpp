#include "GameObject.h"

#include "Road.h"
#include "Utils.h"
#include "Camera.h"
#include <SDL_rect.h>
#include "ModuleRenderer.h"

GameObject::GameObject(const WorldPosition& position, const Road* road, Texture* texture) :
	position(position), road(road), texture(texture)
{
	size.w = (float)texture->r.w * SPRITE_SIZE_RATIO;
	size.h = (float)texture->r.h * SPRITE_SIZE_RATIO;

	limitZ();
}

GameObject::~GameObject()
{ }

const WorldPosition* GameObject::getPosition() const
{
	return &position;
}

void GameObject::elevate(float incY)
{
	position.y += incY;
}

void GameObject::update(float deltaTimeS)
{ }

void GameObject::render(float xOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short clipY) const
{
	if(camera->isBehind(zOffset + position.z)) return;

	WorldPosition worldPositionBottomLeft{ position.x - size.w / 2.0f - xOffset, position.y, position.z + zOffset };
	WorldPosition worldPositionBottomRight{ position.x + size.w / 2.0f - xOffset, position.y, position.z + zOffset };

	WindowPosition windowPositionBottomLeft;
	WindowPosition windowPositionBottomRight;
	
	camera->project(worldPositionBottomLeft, windowPositionBottomLeft);
	camera->project(worldPositionBottomRight, windowPositionBottomRight);

	if(windowPositionBottomRight.x <= 0) return;
	if(windowPositionBottomLeft.x >= WINDOW_WIDTH) return;

	if(windowPositionBottomLeft.y <= 0) return;

	SDL_Rect dst;

	dst.x = windowPositionBottomLeft.x;
	dst.w = windowPositionBottomRight.x - windowPositionBottomLeft.x;

	dst.h = (int)(dst.w * size.h / size.w);
	dst.y = windowPositionBottomLeft.y - dst.h;

	if(dst.y >= WINDOW_HEIGHT) return;

	SDL_Rect src = texture->r;

	if(dst.y + dst.h > clipY)
	{
		int dstH = dst.h;

		if((dst.h = clipY - dst.y) < 0) dst.h = 0;

		float ratio = (float)dst.h / dstH;

		src.h = (int)(src.h * ratio);
	}

	moduleRenderer->renderTexture(texture->t, src, dst);
}

void GameObject::limitZ()
{
	position.z = modF0ToL(position.z, road->getLength());
}