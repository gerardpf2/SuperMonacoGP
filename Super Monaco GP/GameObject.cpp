#include "GameObject.h"

#include "Road.h"
#include "Utils.h"
#include "Colors.h"
#include "Camera.h"
#include "Segment.h"
#include "ModuleRenderer.h"

GameObject::GameObject(const Position3f& position, const Road* road) :
	position(position), road(road)
{ }

GameObject::~GameObject()
{ }

const Position3f* GameObject::getPosition() const
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

void GameObject::render(float xOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short clipY) const
{
	if(camera->getIsBehind(zOffset + position.third)) return;

	float width = 1.0f;
	float height = 20.0f;

	Position3f worldPositionBottomLeft{ position.first - width / 2.0f - xOffset, position.second, position.third + zOffset };
	Position3f worldPositionBottomRight{ position.first + width / 2.0f - xOffset, position.second, position.third + zOffset };
	Position3f worldPositionUpLeft{ position.first - width / 2.0f - xOffset, position.second + height, position.third + zOffset };
	Position3f worldPositionUpRight{ position.first + width / 2.0f - xOffset, position.second + height, position.third + zOffset };

	Position2s screenPositionBottomLeft, screenPositionBottomRight;
	Position2s screenPositionUpLeft, screenPositionUpRight;

	camera->getPositionWorldToScreen(worldPositionBottomLeft, screenPositionBottomLeft);
	camera->getPositionWorldToScreen(worldPositionBottomRight, screenPositionBottomRight);
	camera->getPositionWorldToScreen(worldPositionUpLeft, screenPositionUpLeft);
	camera->getPositionWorldToScreen(worldPositionUpRight, screenPositionUpRight);

	if(screenPositionBottomLeft.second > clipY)
		screenPositionBottomLeft.second = clipY;

	if(screenPositionBottomRight.second > clipY)
		screenPositionBottomRight.second = clipY;

	if(screenPositionUpLeft.second > clipY)
		screenPositionUpLeft.second = clipY;

	if(screenPositionUpRight.second > clipY)
		screenPositionUpRight.second = clipY;

	if(screenPositionBottomRight.first <= 0 && screenPositionUpRight.first <= 0) return;
	if(screenPositionBottomLeft.first >= WINDOW_WIDTH && screenPositionUpLeft.first >= WINDOW_WIDTH) return;

	if(screenPositionBottomLeft.second <= 0 || screenPositionUpLeft.second >= WINDOW_HEIGHT) return;

	moduleRenderer->renderTrapezoid(screenPositionBottomLeft, screenPositionBottomRight, screenPositionUpRight, screenPositionUpLeft, RED, true);
}

void GameObject::updateLimitZRoad()
{
	position.third = modF0ToL(position.third, road->getLength());
}