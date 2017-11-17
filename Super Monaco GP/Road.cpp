#include "Road.h"

#include "Utils.h"
#include "Colors.h"
#include "Camera.h"
#include "Segment.h"
#include "GameObject.h"

Road::Road(float length)
{
	if(length < ROAD_MIN_LENGTH)
		length = ROAD_MIN_LENGTH;

	this->length = length;

	// this->length = 40.0f; //

	unsigned int nSegments = (unsigned int)ceilf(this->length / SEGMENT_LENGTH);
	segments.reserve(nSegments);

	unsigned int colorIndex = 0;
	unsigned int colors[2]{ BLACK, WHITE };

	float currentZNear = 0.0f;

	for(unsigned int i = 0; i < nSegments; ++i)
	{
		Segment* segment = new Segment(i, currentZNear, colors[(colorIndex++) % 2]);

		segments.push_back(segment);
		currentZNear = segment->getZFar();
	}

	segments[0]->color = GREEN;
	segments[segments.size() - 1]->color = RED;

	addHill(10.0f, 20.0f, 10.0f, 20.0f, 10.0f);

	addCurve(10.0f, 20.0f, 10.0f, 20.0f, 0.05f);

	addHill(70.0f, 20.0f, 10.0f, 20.0f, -10.0f);

	addCurve(70.0f, 20.0f, 10.0f, 20.0f, -0.05f);

	segments[getSegmentAtZ(10.0f)->getIndex()]->color = BLUE; // enter
	segments[getSegmentAtZ(30.0f)->getIndex()]->color = BLUE; // hold
	segments[getSegmentAtZ(40.0f)->getIndex()]->color = BLUE; // leave
	segments[getSegmentAtZ(60.0f)->getIndex()]->color = BLUE; // exit

	segments[getSegmentAtZ(70.0f)->getIndex()]->color = YELLOW; // enter
	segments[getSegmentAtZ(90.0f)->getIndex()]->color = YELLOW; // hold
	segments[getSegmentAtZ(100.0f)->getIndex()]->color = YELLOW; // leave
	segments[getSegmentAtZ(120.0f)->getIndex()]->color = YELLOW; // exit
}

Road::~Road()
{ }

float Road::getLength() const
{
	return length;
}

Segment* Road::getSegmentAtZ(float z) const
{
	return getSegment((int)(z / SEGMENT_LENGTH));
}

void Road::addHill(float zStart, float enterLength, float holdLength, float leaveLength, float value) const
{
	float zEnter = zStart;
	float zHold = zEnter + enterLength;
	float zLeave = zHold + holdLength;
	float zExit = zLeave + leaveLength;

	unsigned int indexEnter = getSegmentAtZ(zEnter)->getIndex();
	unsigned int indexHold = getSegmentAtZ(zHold)->getIndex();
	unsigned int indexLeave = getSegmentAtZ(zLeave)->getIndex();
	unsigned int indexExit = getSegmentAtZ(zExit)->getIndex();

	addHillEnter(indexEnter, indexHold, enterLength, value);
	addHillHold(indexHold, indexLeave, holdLength, value);
	addHillLeave(indexLeave, indexExit, leaveLength, value);
}

void Road::addCurve(float zStart, float enterLength, float holdLength, float leaveLength, float value) const
{
	float zEnter = zStart;
	float zHold = zEnter + enterLength;
	float zLeave = zHold + holdLength;
	float zExit = zLeave + leaveLength;

	unsigned int indexEnter = getSegmentAtZ(zEnter)->getIndex();
	unsigned int indexHold = getSegmentAtZ(zHold)->getIndex();
	unsigned int indexLeave = getSegmentAtZ(zLeave)->getIndex();
	unsigned int indexExit = getSegmentAtZ(zExit)->getIndex();

	addCurveEnter(indexEnter, indexHold, enterLength, value);
	addCurveHold(indexHold, indexLeave, holdLength, value);
	addCurveLeave(indexLeave, indexExit, leaveLength, value);
}

void Road::render(const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	short maxScreenY = WINDOW_HEIGHT;

	float zOffset = 0.0f;
	float zBase = camera->getPosition()->third;

	Segment* segment = getSegmentAtZ(zBase);

	float x = 0.0f;
	float dX = segment->dX - segment->dX * fmodf(zBase, SEGMENT_LENGTH) / SEGMENT_LENGTH;

	for(unsigned int i = 0; i < N_SEGMENTS_DRAW; ++i)
	{
		segment->clipY = maxScreenY;
		segment->render(x, dX, zOffset, camera, moduleRenderer, maxScreenY);

		if(segment->getIndex() == (unsigned int)(segments.size() - 1)) zOffset += length;

		x += dX;
		dX += segment->dX;

		segment = getSegment(segment->getIndex() + 1);
	}

	// GameObjects

	zOffset = 0.0f;
	zBase = camera->getPosition()->third;

	segment = getSegmentAtZ(zBase);

	x = 0.0f;
	dX = segment->dX - segment->dX * fmodf(zBase, SEGMENT_LENGTH) / SEGMENT_LENGTH;

	for(int i = (int)N_SEGMENTS_DRAW - 1; i >= 0; --i)
	{
		for(unsigned int i = 0; i < (unsigned int)segment->gameObjects.size(); ++i)
		{
			segment->gameObjects[i]->render(x, zOffset, camera, moduleRenderer, segment->clipY);
		}

		if(segment->getIndex() == (unsigned int)(segments.size() - 1)) zOffset += length;

		x += dX;
		dX += segment->dX;

		segment = getSegment(segment->getIndex() + 1);

		/* segment->clipY = maxScreenY;
		segment->render(x, dX, zOffset, camera, moduleRenderer, maxScreenY);

		if(segment->getIndex() == (unsigned int)(segments.size() - 1)) zOffset += length;

		x += dX;
		dX += segment->dX;

		segment = getSegment(segment->getIndex() + 1); */
	}
}

void Road::clear()
{
	for(int i = (int)segments.size() - 1; i >= 0; --i)
	{
		delete segments[i];
		segments[i] = nullptr;
	}

	segments.clear();
}

Segment* Road::getSegment(int index) const
{
	return segments[modI0ToL(index, segments.size())];
}

void Road::addHillEnter(unsigned int indexStart, unsigned int indexEnd, float enterLength, float value) const
{
	float c = 0.0f;
	float nSegments = enterLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++i, ++c)
	{
		Segment* segment = getSegment(i);

		segment->yNear = getSegment((int)segment->getIndex() - 1)->yFar;
		segment->yFar = ease(0.0f, value, c / nSegments);
	}
}

void Road::addHillHold(unsigned int indexStart, unsigned int indexEnd, float holdLength, float value) const
{
	float c = 0.0f;
	float nSegments = holdLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++i, ++c)
	{
		Segment* segment = getSegment(i);

		segment->yNear = getSegment((int)segment->getIndex() - 1)->yFar;
		segment->yFar = ease(segment->yNear, value, c / nSegments);
	}
}

void Road::addHillLeave(unsigned int indexStart, unsigned int indexEnd, float leaveLength, float value) const
{
	float c = 0.0f;
	float nSegments = leaveLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++i, ++c)
	{
		Segment* segment = getSegment(i);

		segment->yNear = getSegment((int)segment->getIndex() - 1)->yFar;
		segment->yFar = ease(value, 0.0f, c / nSegments);
	}
}

void Road::addCurveEnter(unsigned int indexStart, unsigned int indexEnd, float enterLength, float value) const
{
	float c = 0.0f;
	float nSegments = enterLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++i, ++c)
		getSegment(i)->dX = ease(0.0f, value, c / nSegments);
}

void Road::addCurveHold(unsigned int indexStart, unsigned int indexEnd, float holdLength, float value) const
{
	for(unsigned int i = indexStart; i != indexEnd; ++i)
		getSegment(i)->dX = value;
}

void Road::addCurveLeave(unsigned int indexStart, unsigned int indexEnd, float leaveLength, float value) const
{
	float c = 0.0f;
	float nSegments = leaveLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++i, ++c)
		getSegment(i)->dX = ease(value, 0.0f, c / nSegments);
}