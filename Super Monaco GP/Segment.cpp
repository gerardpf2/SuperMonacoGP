#include "Segment.h"

#include "Utils.h"
#include "Camera.h"
#include "Globals.h"
#include "ModuleRenderer.h"

using namespace std;

Segment::Segment(uint index, float zNear) :
	index(index), zNear(zNear), zFar(zNear + SEGMENT_LENGTH)
{ }

Segment::~Segment()
{ }

uint Segment::getIndex() const
{
	return index;
}

float Segment::getZNear() const
{
	return zNear;
}

float Segment::getZFar() const
{
	return zFar;
}

float Segment::getCurve() const
{
	return curve;
}

void Segment::setCurve(float curve)
{
	this->curve = curve;
}

float Segment::getYNear() const
{
	return yNear;
}

void Segment::setYNear(float yNear)
{
	this->yNear = yNear;
}

float Segment::getYFar() const
{
	return yFar;
}

void Segment::setYFar(float yFar)
{
	this->yFar = yFar;
}

float Segment::getXOffsetNear() const
{
	return xOffsetNear;
}

void Segment::setXOffsetNear(float xOffsetNear)
{
	this->xOffsetNear = xOffsetNear;
}

float Segment::getXOffsetFar() const
{
	return xOffsetFar;
}

void Segment::setXOffsetFar(float xOffsetFar)
{
	this->xOffsetFar = xOffsetFar;
}

float Segment::getZOffset() const
{
	return zOffset;
}

void Segment::setZOffset(float zOffset)
{
	this->zOffset = zOffset;
}

short Segment::getClipY() const
{
	return clipY;
}

void Segment::setClipY(short clipY)
{
	this->clipY = clipY;
}

const RumbleColors* Segment::getRumbleColors() const
{
	return rumbleColors;
}

void Segment::setRumbleColors(const RumbleColors* rumbleColors)
{
	this->rumbleColors = rumbleColors;
}

const set<const GameObject*>* Segment::getGameObjects() const
{
	return &gameObjects;
}

void Segment::addGameObject(const GameObject* gameObject)
{
	gameObjects.insert(gameObject);
}

void Segment::eraseGameObject(const GameObject* gameObject)
{
	gameObjects.erase(gameObject);
}

void Segment::render(float xOffset, float dXOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxWindowY) const
{
	// Check if this segment is behind the camera

	if(camera->isBehind(zOffset + zFar)) return;

	// Project near and far points of this segment

	WorldPosition worldPositionNear{ -xOffset, yNear, zOffset + zNear };
	WorldPosition worldPositionFar{ -xOffset - dXOffset, yFar, zOffset + zFar };

	WindowPosition windowPositionNear, windowPositionFar;

	camera->project(worldPositionNear, windowPositionNear);
	camera->project(worldPositionFar, windowPositionFar);

	// Check if the projected points are outside the window rect (y)

	if(windowPositionFar.y >= maxWindowY) return;
	maxWindowY = windowPositionFar.y;

	if(windowPositionNear.y <= windowPositionFar.y) return;
	if(windowPositionNear.y <= 0 || windowPositionFar.y >= WINDOW_HEIGHT) return;

	// Some parts of this segment are visible

	// Segment parts 0 and 3 ---

	WorldTrapezoid worldTrapezoid3;

	worldTrapezoid3.nl = worldPositionNear;
	worldTrapezoid3.nr = worldPositionNear;
	worldTrapezoid3.fr = worldPositionFar;
	worldTrapezoid3.fl = worldPositionFar;

	worldTrapezoid3.nl.x += ROAD_MIN_X;
	worldTrapezoid3.nr.x += ROAD_MAX_X;
	worldTrapezoid3.fr.x += ROAD_MAX_X;
	worldTrapezoid3.fl.x += ROAD_MIN_X;

	WindowTrapezoid windowTrapezoid3;

	camera->project(worldTrapezoid3, windowTrapezoid3);

	WindowTrapezoid windowTrapezoid0 = windowTrapezoid3;

	windowTrapezoid0.nl.x = windowTrapezoid0.fl.x = 0;
	windowTrapezoid0.nr.x = windowTrapezoid0.fr.x = WINDOW_WIDTH;

	moduleRenderer->renderTrapezoid(windowTrapezoid0, rumbleColors->a);

	if(isOutsideWindowX(windowTrapezoid3)) return;

	moduleRenderer->renderTrapezoid(windowTrapezoid3, rumbleColors->d);

	// --- Segment parts 0 and 3

	// Segment parts 1L and 1R ---

	WorldTrapezoid worldTrapezoid1L = worldTrapezoid3;

	worldTrapezoid1L.nr.x = worldTrapezoid1L.nl.x + ROAD_RUMBLE_WIDTH;
	worldTrapezoid1L.fr.x = worldTrapezoid1L.fl.x + ROAD_RUMBLE_WIDTH;

	WindowTrapezoid windowTrapezoid1L;

	camera->project(worldTrapezoid1L, windowTrapezoid1L);

	if(!(isOutsideWindowX(windowTrapezoid1L)))
		moduleRenderer->renderTrapezoid(windowTrapezoid1L, rumbleColors->b);
	
	WorldTrapezoid worldTrapezoid1R = worldTrapezoid3;

	worldTrapezoid1R.nl.x = worldTrapezoid1R.nr.x - ROAD_RUMBLE_WIDTH;
	worldTrapezoid1R.fl.x = worldTrapezoid1R.fr.x - ROAD_RUMBLE_WIDTH;

	WindowTrapezoid windowTrapezoid1R;

	camera->project(worldTrapezoid1R, windowTrapezoid1R);

	if(!(isOutsideWindowX(windowTrapezoid1R)))
		moduleRenderer->renderTrapezoid(windowTrapezoid1R, rumbleColors->b);

	// --- Segment parts 1L and 1R

	// Segment parts 2L and 2R ---

	WorldTrapezoid worldTrapezoid2L = worldTrapezoid1L;

	worldTrapezoid2L.nl.x = worldTrapezoid2L.nr.x + ROAD_RUMBLE_OFFSET_X_PARTS_1_2;
	worldTrapezoid2L.fl.x = worldTrapezoid2L.fr.x + ROAD_RUMBLE_OFFSET_X_PARTS_1_2;
	
	worldTrapezoid2L.fr.x = worldTrapezoid2L.fl.x + ROAD_RUMBLE_WIDTH_PART_2;
	worldTrapezoid2L.nr.x = worldTrapezoid2L.nl.x + ROAD_RUMBLE_WIDTH_PART_2;

	WindowTrapezoid windowTrapezoid2L;

	camera->project(worldTrapezoid2L, windowTrapezoid2L);

	if(!(isOutsideWindowX(windowTrapezoid2L)))
		moduleRenderer->renderTrapezoid(windowTrapezoid2L, rumbleColors->c);

	WorldTrapezoid worldTrapezoid2R = worldTrapezoid1R;

	worldTrapezoid2R.nr.x = worldTrapezoid2R.nl.x - ROAD_RUMBLE_OFFSET_X_PARTS_1_2;
	worldTrapezoid2R.fr.x = worldTrapezoid2R.fl.x - ROAD_RUMBLE_OFFSET_X_PARTS_1_2;
	
	worldTrapezoid2R.nl.x = worldTrapezoid2R.nr.x - ROAD_RUMBLE_WIDTH_PART_2;
	worldTrapezoid2R.fl.x = worldTrapezoid2R.fr.x - ROAD_RUMBLE_WIDTH_PART_2;

	WindowTrapezoid windowTrapezoid2R;

	camera->project(worldTrapezoid2R, windowTrapezoid2R);

	if(!(isOutsideWindowX(windowTrapezoid2R)))
		moduleRenderer->renderTrapezoid(windowTrapezoid2R, rumbleColors->c);

	// --- Segment parts 2L and 2R

	// Segment part 4 ---
	
	WorldTrapezoid worldTrapezoid4 = worldTrapezoid3;

	worldTrapezoid4.nl.x = worldTrapezoid4.nl.x + ROAD_WIDTH / 2.0f - ROAD_RUMBLE_WIDTH_PART_4 / 2.0f;
	worldTrapezoid4.fl.x = worldTrapezoid4.fl.x + ROAD_WIDTH / 2.0f - ROAD_RUMBLE_WIDTH_PART_4 / 2.0f;

	worldTrapezoid4.nr.x = worldTrapezoid4.nl.x + ROAD_RUMBLE_WIDTH_PART_4;
	worldTrapezoid4.fr.x = worldTrapezoid4.fl.x + ROAD_RUMBLE_WIDTH_PART_4;

	WindowTrapezoid windowTrapezoid4;

	camera->project(worldTrapezoid4, windowTrapezoid4);

	if(!(isOutsideWindowX(windowTrapezoid4)))
		moduleRenderer->renderTrapezoid(windowTrapezoid4, rumbleColors->e);

	// --- Segment part 4
}