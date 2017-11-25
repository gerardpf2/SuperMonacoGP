#include "GameObject.h"

#include "Road.h"
#include "Utils.h"
#include "Camera.h"
#include "Segment.h"
#include <SDL_rect.h>
#include "Animation.h"
#include "ModuleRenderer.h"

using namespace std;

// GameObject::GameObject(const WorldPosition& position, const Texture* texture, const Road* road) :
GameObject::GameObject(const WorldPosition& position, const vector<Animation*>* animations, const Road* road) :
	position(position), animations(animations), road(road)
{
	// Revisar Size

	// size.w = (float)texture->r.w * SPRITE_SIZE_RATIO;
	// size.h = (float)texture->r.h * SPRITE_SIZE_RATIO;
	size.w = (float)(*animations)[currentAnimation]->getCurrentFrame()->r.w * SPRITE_SIZE_RATIO;
	size.h = (float)(*animations)[currentAnimation]->getCurrentFrame()->r.h * SPRITE_SIZE_RATIO;

	limitZ();
}

GameObject::~GameObject()
{ }

const WorldPosition* GameObject::getPosition() const
{
	return &position;
}

/* const Texture* GameObject::getTexture() const
{
	return texture;
} */

const vector<Animation*>* GameObject::getAnimations() const
{
	return animations;
}

const Road* GameObject::getRoad() const
{
	return road;
}

void GameObject::elevate()
{
	Segment* segment = road->getSegmentAtZ(position.z);

	position.y += interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar());
}

void GameObject::update(float deltaTimeS)
{
	(*animations)[currentAnimation]->update(deltaTimeS);
}

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

	// SDL_Rect src = texture->r;
	const Texture* texture = (*animations)[currentAnimation]->getCurrentFrame();

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