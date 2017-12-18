#include "GameObject.h"

#include "Road.h"
#include "Utils.h"
#include "Camera.h"
#include "Segment.h"
#include <SDL_rect.h>
#include "ModuleWorld.h"
#include "ModuleRenderer.h"

#include "GameEngine.h"
#include "ModuleCollision.h"

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

const Collider* GameObject::getCollider() const
{
	return &collider;
}

/* const Road* GameObject::getRoad() const
{
	return road;
}

void GameObject::setRoad(const Road* road)
{
	this->road = road;
} */

const ModuleWorld* GameObject::getModuleWorld() const
{
	return moduleWorld;
}

void GameObject::setModuleWorld(const ModuleWorld* moduleWorld)
{
	this->moduleWorld = moduleWorld;

	// Register collider

	collider.g = this;
	collider.b = Box{ 0.5f * size.w, size.h, 0.5f }; //

	if(collider.b.d > 0.0f)
		getModuleWorld()->getGameEngine()->getModuleCollision()->addCollider(&collider);
}

void GameObject::elevate()
{
	Segment* segment = moduleWorld->getRoad()->getSegmentAtZ(position.z);

	position.y += interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar());
}

/* void GameObject::moveX(float incX)
{
	position.x += incX;
} */

void GameObject::move(const WorldPosition& offset)
{
	position.x += offset.x;
	position.y += offset.y;
	position.z += offset.z;
}

void GameObject::update(float deltaTimeS)
{ }

void GameObject::render(const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	Segment* segment = moduleWorld->getRoad()->getSegmentAtZ(position.z);

	float xOffset;

	if(camera->getForward())
		xOffset = interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getXOffsetNear(), segment->getXOffsetFar());
	else
		xOffset = interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getXOffsetFar(), segment->getXOffsetNear());

	float zOffset = segment->getZOffset();
	short clipY = segment->getClipY();

	if(camera->isBehind(zOffset + position.z)) return;

	WorldPosition worldPositionBottomLeft{ position.x - size.w / 2.0f + xOffset, position.y, position.z + zOffset };
	WorldPosition worldPositionBottomRight{ position.x + size.w / 2.0f + xOffset, position.y, position.z + zOffset };

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

	const Texture* texture = getCurrentTexture(!camera->getForward());

	SDL_Rect src = *texture->r;

	if(dst.y + dst.h > clipY)
	{
		int dstH = dst.h;

		if((dst.h = clipY - dst.y) < 0) dst.h = 0;

		float ratio = (float)dst.h / dstH;

		src.h = (int)(src.h * ratio);
	}

	moduleRenderer->renderTexture(texture->t, &src, &dst, texture->hFlipped);

	_DEBUG_renderCollider_DEBUG_(xOffset, zOffset, clipY, camera, moduleRenderer);
}

void GameObject::cleanUp()
{ }

void GameObject::limitZ()
{
	position.z = mod0L(position.z, moduleWorld->getRoad()->getLength());
}

void GameObject::_DEBUG_renderCollider_DEBUG_(float xOffset, float zOffset, short clipY, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	float x0 = position.x - collider.b.w / 2.0f + xOffset;
	float y0 = position.y;
	float z0 = position.z - collider.b.d / 2.0f + zOffset;

	float x1 = x0 + collider.b.w;
	float y1 = y0 + collider.b.h;
	float z1 = z0 + collider.b.d;

	WorldTrapezoid face0{ WorldPosition{ x0, y0, z0 }, WorldPosition{ x1, y0, z0 }, WorldPosition{ x1, y1, z0 }, WorldPosition{ x0, y1, z0 } };
	WorldTrapezoid face1{ WorldPosition{ x1, y0, z0 }, WorldPosition{ x1, y0, z1 }, WorldPosition{ x1, y1, z1 }, WorldPosition{ x1, y1, z0 } };
	WorldTrapezoid face2{ WorldPosition{ x0, y0, z0 }, WorldPosition{ x0, y0, z1 }, WorldPosition{ x0, y1, z1 }, WorldPosition{ x0, y1, z0 } };
	WorldTrapezoid face3{ WorldPosition{ x0, y0, z1 }, WorldPosition{ x1, y0, z1 }, WorldPosition{ x1, y1, z1 }, WorldPosition{ x0, y1, z1 } };

	_DEBUG_renderColliderFace_DEBUG_(face3, clipY, 0x330000FF, camera, moduleRenderer);
	_DEBUG_renderColliderFace_DEBUG_(face2, clipY, 0x3300FF00, camera, moduleRenderer);
	_DEBUG_renderColliderFace_DEBUG_(face1, clipY, 0x3300FF00, camera, moduleRenderer);
	_DEBUG_renderColliderFace_DEBUG_(face0, clipY, 0x330000FF, camera, moduleRenderer);
}

void GameObject::_DEBUG_renderColliderFace_DEBUG_(const WorldTrapezoid& worldTrapezoid, short clipY, uint color, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	WindowTrapezoid windowTrapezoid;
	camera->project(worldTrapezoid, windowTrapezoid);

	bool toBeRendered = !((windowTrapezoid.fl.y >= clipY) || (windowTrapezoid.nl.y <= windowTrapezoid.fl.y) || (windowTrapezoid.nl.y <= 0 || windowTrapezoid.fl.y >= WINDOW_HEIGHT));

	if(toBeRendered && !isOutsideWindowX(windowTrapezoid))
	{
		moduleRenderer->renderTrapezoid(windowTrapezoid, color);
		moduleRenderer->renderTrapezoid(windowTrapezoid, 0xFFFFFFFF, false);
	}
}