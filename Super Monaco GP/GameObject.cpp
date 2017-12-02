#include "GameObject.h"

#include "Road.h"
#include "Utils.h"
#include "Camera.h"
#include "Segment.h"
#include <SDL_rect.h>
#include "ModuleRenderer.h"

GameObject::GameObject(uint id) :
	id(id)
{ }

GameObject::~GameObject()
{ }

uint GameObject::getId() const
{
	return id;
}

const WorldPosition* GameObject::getPosition() const
{
	return &position;
}

void GameObject::setPosition(const WorldPosition& position)
{
	this->position = position;

	limitZ();
}

const Road* GameObject::getRoad() const
{
	return road;
}

void GameObject::setRoad(const Road* road)
{
	this->road = road;
}

void GameObject::elevate()
{
	Segment* segment = road->getSegmentAtZ(position.z);

	position.y += interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar());
}

void GameObject::moveX(float incX)
{
	position.x += incX;
}

void GameObject::update(float deltaTimeS)
{ }

void GameObject::render(const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	Segment* segment = road->getSegmentAtZ(position.z);

	float xOffset = interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getXOffsetNear(), segment->getXOffsetFar());
	float zOffset = segment->getZOffset();
	short clipY = segment->getClipY();

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

	const Texture* texture = getCurrentTexture();

	SDL_Rect src = *texture->r;

	if(dst.y + dst.h > clipY)
	{
		int dstH = dst.h;

		if((dst.h = clipY - dst.y) < 0) dst.h = 0;

		float ratio = (float)dst.h / dstH;

		src.h = (int)(src.h * ratio);
	}

	moduleRenderer->renderTexture(texture->t, src, dst);
}

void GameObject::cleanUp()
{ }

void GameObject::limitZ()
{
	position.z = mod0L(position.z, road->getLength());
}