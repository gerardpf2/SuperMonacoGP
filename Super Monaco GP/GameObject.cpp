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

GameObject::GameObject(/* uint id */) // :
	// id(id)
{ }

GameObject::~GameObject()
{ }

/* uint GameObject::getId() const
{
	return id;
} */

float GameObject::getGlobalZ() const
{
	return globalZ;
}

const WorldPosition* GameObject::getPosition() const
{
	return &position;
}

void GameObject::setPosition(const WorldPosition& position)
{
	this->position = position;

	limitZ();

	globalZ = position.z;
}

const Box* GameObject::getBox() const
{
	return &box;
}

/* const Collider* GameObject::getCollider() const
{
	return &collider;
} */

void GameObject::setBox(const Box& box)
{
	// collider.b = box;
	this->box = box;
}

void GameObject::defineBox(float mW, float d)
{
	// collider.b = Box{ mW * size.w, size.h, d };
	box = Box{ mW * size.w, size.h, d };
}

/* void GameObject::enableCollider()
{
	collider.g = this;
	getModuleWorld()->getGameEngine()->getModuleCollision()->addCollider(&collider);
} */

ModuleWorld* GameObject::getModuleWorld() const
{
	return moduleWorld;
}

void GameObject::setModuleWorld(ModuleWorld* moduleWorld)
{
	this->moduleWorld = moduleWorld;
}

void GameObject::elevate()
{
	Segment* segment = moduleWorld->getRoad()->getSegmentAtZ(position.z);

	position.y += interpolate(position.z, segment->getZNear(), segment->getZFar(), segment->getYNear(), segment->getYFar());
}

void GameObject::move(const WorldPosition& offset)
{
	position.x += offset.x;
	position.y += offset.y;
	position.z += offset.z;

	globalZ += offset.z;
}

bool GameObject::getRenderCollisionBox() const
{
	return renderBox;
}

void GameObject::setRenderCollisionBox(bool renderCollisionBox)
{
	renderBox = renderCollisionBox;
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

	if(renderBox) renderCollisionBox(xOffset, zOffset, clipY, camera, moduleRenderer);
}

void GameObject::cleanUp()
{ }

void GameObject::limitZ()
{
	position.z = mod0L(position.z, moduleWorld->getRoad()->getLength());
}

void GameObject::renderCollisionBox(float xOffset, float zOffset, short clipY, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	float xnl = position.x - box.w / 2.0f + xOffset;
	float xnr = xnl + box.w;
	
	float ynb = position.y; // - 5.0f;
	float ynt = position.y + box.h;

	float zn = position.z + zOffset;
	float zf = zn + box.d;

	WorldTrapezoid face0{ WorldPosition{ xnl, ynb, zn }, WorldPosition{ xnr, ynb, zn }, WorldPosition{ xnr, ynt, zn }, WorldPosition{ xnl, ynt, zn } };
	WorldTrapezoid face1{ WorldPosition{ xnr, ynb, zn }, WorldPosition{ xnr, ynb, zf }, WorldPosition{ xnr, ynt, zf }, WorldPosition{ xnr, ynt, zn } };
	WorldTrapezoid face2{ WorldPosition{ xnl, ynb, zn }, WorldPosition{ xnl, ynb, zf }, WorldPosition{ xnl, ynt, zf }, WorldPosition{ xnl, ynt, zn } };
	WorldTrapezoid face3{ WorldPosition{ xnl, ynb, zf }, WorldPosition{ xnr, ynb, zf }, WorldPosition{ xnr, ynt, zf }, WorldPosition{ xnl, ynt, zf } };
	WorldTrapezoid face4{ WorldPosition{ xnl, ynt, zn }, WorldPosition{ xnr, ynt, zn }, WorldPosition{ xnr, ynt, zf }, WorldPosition{ xnl, ynt, zf } };
	WorldTrapezoid face5{ WorldPosition{ xnl, ynb, zn }, WorldPosition{ xnr, ynb, zn }, WorldPosition{ xnr, ynb, zf }, WorldPosition{ xnl, ynb, zf } };

	if(camera->getForward())
	{
		renderCollisionBoxFace(face5, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face3, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face2, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face1, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face0, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face4, clipY, 0x55FFFFFF, camera, moduleRenderer);
	}
	else
	{
		renderCollisionBoxFace(face4, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face0, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face1, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face2, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face3, clipY, 0x55FFFFFF, camera, moduleRenderer);
		renderCollisionBoxFace(face5, clipY, 0x55FFFFFF, camera, moduleRenderer);
	}
}

void GameObject::renderCollisionBoxFace(const WorldTrapezoid& worldTrapezoid, short clipY, uint color, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	WindowTrapezoid windowTrapezoid;
	camera->project(worldTrapezoid, windowTrapezoid);

	bool toBeRendered = !camera->isBehind(worldTrapezoid.fl.z + box.d);
	toBeRendered &= !(windowTrapezoid.fl.y >= clipY);
	toBeRendered &= !(windowTrapezoid.nl.y <= windowTrapezoid.fl.y);
	toBeRendered &= !(windowTrapezoid.nl.y <= 0 || windowTrapezoid.fl.y >= WINDOW_HEIGHT);

	if(toBeRendered && !isOutsideWindowX(windowTrapezoid))
	{
		moduleRenderer->renderTrapezoid(windowTrapezoid, color);
		moduleRenderer->renderTrapezoid(windowTrapezoid, 0xFF000000, false);
	}
}