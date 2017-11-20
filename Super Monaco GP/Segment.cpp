#include "Segment.h"

#include "Camera.h"
#include "Globals.h"
#include "ModuleRenderer.h"

Segment::Segment(unsigned int index, float zNear /* , unsigned int color */) :
	index(index), zNear(zNear), zFar(zNear + SEGMENT_LENGTH) //, color(color)
{ }

Segment::~Segment()
{ }

unsigned int Segment::getIndex() const
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

#include "Colors.h"

void Segment::render(float xOffset, float dXOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxScreenY) const
{
	if(camera->getIsBehind(zOffset + zFar)) return;

	WorldPosition worldPositionNearLeft3{ ROAD_MIN_X - xOffset, yNear, zOffset + zNear };
	WorldPosition worldPositionNearRight3{ ROAD_MAX_X - xOffset, yNear, zOffset + zNear };
	WorldPosition worldPositionFarLeft3{ ROAD_MIN_X - xOffset - dXOffset, yFar, zOffset + zFar };
	WorldPosition worldPositionFarRight3{ ROAD_MAX_X - xOffset - dXOffset, yFar, zOffset + zFar };

	ScreenPosition screenPositionNearLeft3, screenPositionNearRight3;
	ScreenPosition screenPositionFarLeft3, screenPositionFarRight3;

	camera->project(worldPositionNearLeft3, worldPositionNearRight3, worldPositionFarLeft3,  worldPositionFarRight3, screenPositionNearLeft3, screenPositionNearRight3, screenPositionFarLeft3, screenPositionFarRight3);

	if(screenPositionFarLeft3.second >= maxScreenY) return;
	maxScreenY = screenPositionFarLeft3.second;

	// Part 0 ---

	ScreenPosition screenPositionNearLeft0 = screenPositionNearLeft3;
	ScreenPosition screenPositionNearRight0 = screenPositionNearRight3;
	ScreenPosition screenPositionFarLeft0 = screenPositionFarLeft3;
	ScreenPosition screenPositionFarRight0 = screenPositionFarRight3;

	screenPositionNearLeft0.first = screenPositionFarLeft0.first = 0;
	screenPositionNearRight0.first = screenPositionFarRight0.first = WINDOW_WIDTH;

	moduleRenderer->renderTrapezoid(screenPositionNearLeft0, screenPositionNearRight0, screenPositionFarRight0, screenPositionFarLeft0, rumbleColors->first, true);

	// --- Part 0

	if(screenPositionNearRight3.first <= 0 && screenPositionFarRight3.first <= 0) return;
	if(screenPositionNearLeft3.first >= WINDOW_WIDTH && screenPositionFarLeft3.first >= WINDOW_WIDTH) return;

	if(screenPositionNearLeft3.second <= screenPositionFarLeft3.second) return;
	if(screenPositionNearLeft3.second <= 0 || screenPositionFarLeft3.second >= WINDOW_HEIGHT) return;

	// Part 3 ---

	moduleRenderer->renderTrapezoid(screenPositionNearLeft3, screenPositionNearRight3, screenPositionFarRight3, screenPositionFarLeft3, rumbleColors->fourth, true);

	// --- Part 3

	// Part 1 Left ---

	WorldPosition worldPositionNearLeft1L = worldPositionNearLeft3;
	WorldPosition worldPositionNearRight1L = worldPositionNearRight3;
	WorldPosition worldPositionFarLeft1L = worldPositionFarLeft3;
	WorldPosition worldPositionFarRight1L = worldPositionFarRight3;

	worldPositionNearRight1L.first = worldPositionNearLeft1L.first + ROAD_RUMBLE_WIDTH;
	worldPositionFarRight1L.first = worldPositionFarLeft1L.first + ROAD_RUMBLE_WIDTH;

	ScreenPosition screenPositionNearLeft1L, screenPositionNearRight1L;
	ScreenPosition screenPositionFarLeft1L, screenPositionFarRight1L;

	camera->project(worldPositionNearLeft1L, worldPositionNearRight1L, worldPositionFarLeft1L, worldPositionFarRight1L, screenPositionNearLeft1L, screenPositionNearRight1L, screenPositionFarLeft1L, screenPositionFarRight1L);

	moduleRenderer->renderTrapezoid(screenPositionNearLeft1L, screenPositionNearRight1L, screenPositionFarRight1L, screenPositionFarLeft1L, rumbleColors->second, true);

	// --- Part 1 Left

	// Part 1 Right ---

	WorldPosition worldPositionNearLeft1R = worldPositionNearLeft3;
	WorldPosition worldPositionNearRight1R = worldPositionNearRight3;
	WorldPosition worldPositionFarLeft1R = worldPositionFarLeft3;
	WorldPosition worldPositionFarRight1R = worldPositionFarRight3;

	worldPositionNearLeft1R.first = worldPositionNearRight1R.first - ROAD_RUMBLE_WIDTH;
	worldPositionFarLeft1R.first = worldPositionFarRight1R.first - ROAD_RUMBLE_WIDTH;

	ScreenPosition screenPositionNearLeft1R, screenPositionNearRight1R;
	ScreenPosition screenPositionFarLeft1R, screenPositionFarRight1R;

	camera->project(worldPositionNearLeft1R, worldPositionNearRight1R, worldPositionFarLeft1R, worldPositionFarRight1R, screenPositionNearLeft1R, screenPositionNearRight1R, screenPositionFarLeft1R, screenPositionFarRight1R);

	moduleRenderer->renderTrapezoid(screenPositionNearLeft1R, screenPositionNearRight1R, screenPositionFarRight1R, screenPositionFarLeft1R, rumbleColors->second, true);

	// --- Part 1 Right

	// Part 2 Left ---

	WorldPosition worldPositionNearLeft2L = worldPositionNearLeft1L;
	WorldPosition worldPositionNearRight2L = worldPositionNearRight1L;
	WorldPosition worldPositionFarLeft2L = worldPositionFarLeft1L;
	WorldPosition worldPositionFarRight2L = worldPositionFarRight1L;

	worldPositionNearLeft2L.first = worldPositionNearRight2L.first + 1.0f;
	worldPositionFarLeft2L.first = worldPositionFarRight2L.first + 1.0f;
	worldPositionNearRight2L.first = worldPositionNearLeft2L.first + 1.0f;
	worldPositionFarRight2L.first = worldPositionFarLeft2L.first + 1.0f;

	ScreenPosition screenPositionNearLeft2L, screenPositionNearRight2L;
	ScreenPosition screenPositionFarLeft2L, screenPositionFarRight2L;

	camera->project(worldPositionNearLeft2L, worldPositionNearRight2L, worldPositionFarLeft2L, worldPositionFarRight2L, screenPositionNearLeft2L, screenPositionNearRight2L, screenPositionFarLeft2L, screenPositionFarRight2L);

	moduleRenderer->renderTrapezoid(screenPositionNearLeft2L, screenPositionNearRight2L, screenPositionFarRight2L, screenPositionFarLeft2L, rumbleColors->third, true);

	// --- Part 2 Left

	// Part 2 Right ---

	WorldPosition worldPositionNearLeft2R = worldPositionNearLeft1R;
	WorldPosition worldPositionNearRight2R = worldPositionNearRight1R;
	WorldPosition worldPositionFarLeft2R = worldPositionFarLeft1R;
	WorldPosition worldPositionFarRight2R = worldPositionFarRight1R;

	worldPositionNearRight2R.first = worldPositionNearLeft2R.first - 1.0f;
	worldPositionFarRight2R.first = worldPositionFarLeft2R.first - 1.0f;
	worldPositionNearLeft2R.first = worldPositionNearRight2R.first - 1.0f;
	worldPositionFarLeft2R.first = worldPositionFarRight2R.first - 1.0f;

	ScreenPosition screenPositionNearLeft2R, screenPositionNearRight2R;
	ScreenPosition screenPositionFarLeft2R, screenPositionFarRight2R;

	camera->project(worldPositionNearLeft2R, worldPositionNearRight2R, worldPositionFarLeft2R, worldPositionFarRight2R, screenPositionNearLeft2R, screenPositionNearRight2R, screenPositionFarLeft2R, screenPositionFarRight2R);

	moduleRenderer->renderTrapezoid(screenPositionNearLeft2R, screenPositionNearRight2R, screenPositionFarRight2R, screenPositionFarLeft2R, rumbleColors->third, true);

	// --- Part 2 Right

	// Part 4 ---

	WorldPosition worldPositionNearLeft4 = worldPositionNearLeft3;
	WorldPosition worldPositionNearRight4 = worldPositionNearRight3;
	WorldPosition worldPositionFarLeft4 = worldPositionFarLeft3;
	WorldPosition worldPositionFarRight4 = worldPositionFarRight3;

	worldPositionNearLeft4.first = worldPositionNearLeft3.first + ROAD_WIDTH / 2.0f - 0.5f;
	worldPositionNearRight4.first = worldPositionNearLeft4.first + 1.0f;
	worldPositionFarLeft4.first = worldPositionFarLeft3.first + ROAD_WIDTH / 2.0f - 0.5f;
	worldPositionFarRight4.first = worldPositionFarLeft4.first + 1.0f;

	ScreenPosition screenPositionNearLeft4, screenPositionNearRight4;
	ScreenPosition screenPositionFarLeft4, screenPositionFarRight4;

	camera->project(worldPositionNearLeft4, worldPositionNearRight4, worldPositionFarLeft4, worldPositionFarRight4, screenPositionNearLeft4, screenPositionNearRight4, screenPositionFarLeft4, screenPositionFarRight4);

	moduleRenderer->renderTrapezoid(screenPositionNearLeft4, screenPositionNearRight4, screenPositionFarRight4, screenPositionFarLeft4, rumbleColors->fifth, true);

	// --- Part 4
}