#include "GameObject.h"

#include "Road.h"
#include "Utils.h"
#include "Colors.h"
#include "Camera.h"
#include "Segment.h"
#include "ModuleRenderer.h"

GameObject::GameObject(const WorldPosition& position, const Road* road, TextureInfo& textureInfo) :
	position(position), road(road), textureInfo(textureInfo)
{
	size.first = (float)textureInfo.second.w * SPRITE_SIZE_RATIO;
	size.second = (float)textureInfo.second.h * SPRITE_SIZE_RATIO;

	updateLimitZRoad();
}

/* GameObject::GameObject(const WorldPosition& position, const Road* road, SDL_Texture* texture, SDL_Rect& textureRect) :
	position(position), road(road), texture(texture), textureRect(textureRect)
{
	size.first = 79.0f * 0.05f;
	size.second = 44.0f * 0.05f * 4;
} */

GameObject::~GameObject()
{ }

const WorldPosition* GameObject::getPosition() const
{
	return &position;
}

void GameObject::elevate(float incY)
{
	position.second += incY;
}

void GameObject::update(float deltaTimeS)
{
	/* Segment* segment = road->getSegmentAtZ(position.third);

	float zNear = segment->getZNear();
	float zFar = segment->getZFar();

	float yNear = segment->yNear;
	float yFar = segment->yFar;

	float y = yNear + ((position.third - zNear) / (zFar - zNear)) * (yFar - yNear);

	position.second = y; */
	
	// position.third = modF0ToL(position.third, road->getLength());
}

#include <iostream>

void GameObject::render(float xOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short clipY) const
{
	if(camera->getIsBehind(zOffset + position.third)) return;

	WorldPosition worldPositionBottomLeft{ position.first - size.first / 2.0f - xOffset, position.second, position.third + zOffset };
	WorldPosition worldPositionBottomRight{ position.first + size.first / 2.0f - xOffset, position.second, position.third + zOffset };

	ScreenPosition screenPositionBottomLeft;
	ScreenPosition screenPositionBottomRight;
	
	camera->project(worldPositionBottomLeft, screenPositionBottomLeft);
	camera->project(worldPositionBottomRight, screenPositionBottomRight);

	if(screenPositionBottomRight.first <= 0) return;
	if(screenPositionBottomLeft.first >= WINDOW_WIDTH) return;

	if(screenPositionBottomLeft.second <= 0) return;

	SDL_Rect dst;

	dst.x = screenPositionBottomLeft.first;
	dst.w = screenPositionBottomRight.first - screenPositionBottomLeft.first;

	dst.h = (int)(dst.w * size.second / size.first);
	dst.y = screenPositionBottomLeft.second - dst.h;

	if(dst.y >= WINDOW_HEIGHT) return;

	SDL_Rect src;

	src = textureInfo.second;

	if(dst.y + dst.h > clipY)
	{
		int dstH = dst.h;

		if((dst.h = clipY - dst.y) < 0) dst.h = 0;

		float ratio = (float)dst.h / dstH;

		src.h = (int)(src.h * ratio);
	}

	moduleRenderer->renderTexture(textureInfo.first, src, dst);

	/*
	
	if(camera->getIsBehind(zOffset + position.third)) return;

	// WorldPosition worldPositionBottomLeft{ position.first - width / 2.0f - xOffset, position.second, position.third + zOffset };
	WorldPosition worldPositionBottomRight{ position.first + size.first / 2.0f - xOffset, position.second, position.third + zOffset };
	WorldPosition worldPositionUpLeft{ position.first - size.first / 2.0f - xOffset, position.second + size.second, position.third + zOffset };
	// WorldPosition worldPositionUpRight{ position.first + width / 2.0f - xOffset, position.second + height, position.third + zOffset };

	ScreenPosition screenPositionBottomRight;
	ScreenPosition screenPositionUpLeft;

	// camera->project(worldPositionBottomLeft, screenPositionBottomLeft);
	camera->project(worldPositionBottomRight, screenPositionBottomRight);
	camera->project(worldPositionUpLeft, screenPositionUpLeft);
	// camera->project(worldPositionUpRight, screenPositionUpRight);



	if(screenPositionBottomRight.first <= 0) return;
	if(screenPositionUpLeft.first >= WINDOW_WIDTH) return;

	if(screenPositionBottomRight.second <= 0 || screenPositionUpLeft.second >= WINDOW_HEIGHT) return;

	SDL_Rect dst;

	dst.x = screenPositionUpLeft.first;
	dst.y = screenPositionUpLeft.second;
	dst.w = screenPositionBottomRight.first - screenPositionUpLeft.first;
	dst.h = dst.w * size.second / size.first; // screenPositionBottomRight.second - screenPositionUpLeft.second;

	int a = 2;

	float ratio = 1.0f;

	if(dst.y + dst.h > clipY)
	{
		int dstH = dst.h;

		dst.h = clipY - dst.y;
		if(dst.h < 0) dst.h = 0;

		ratio = (float)dst.h / dstH;

		cout << ratio << endl;
	}

	// float ratio = dst.h / dst.w;

	SDL_Rect src = textureInfo.second;
	
	src.y *= ratio;
	src.h *= ratio;

	moduleRenderer->renderTexture(textureInfo.first, src, dst);

	// moduleRenderer->renderTrapezoid(screenPositionBottomLeft, screenPositionBottomRight, screenPositionUpRight, screenPositionUpLeft, RED, true);

	*/
}

void GameObject::updateLimitZRoad()
{
	position.third = modF0ToL(position.third, road->getLength());
}