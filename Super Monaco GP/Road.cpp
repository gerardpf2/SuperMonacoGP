#include "Road.h"

#include "Camera.h"
#include "Segment.h"

Road::Road(float length)
{
	if(length < ROAD_MIN_LENGTH)
		length = ROAD_MIN_LENGTH;

	this->length = length;

	unsigned int nSegments = (unsigned int)ceilf(length / SEGMENT_LENGTH);
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

	segments[getSegmentAtZ(10.0f)->getIndex()]->color = BLUE; // enter
	segments[getSegmentAtZ(30.0f)->getIndex()]->color = BLUE; // hold
	segments[getSegmentAtZ(40.0f)->getIndex()]->color = BLUE; // leave
	segments[getSegmentAtZ(60.0f)->getIndex()]->color = BLUE; // exit
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
	float zBase = camera->getPosition()->z;

	Segment* segment = getSegmentAtZ(zBase);

	float x = 0.0f;
	float dX = segment->dX - segment->dX * fmodf(zBase, SEGMENT_LENGTH) / SEGMENT_LENGTH;

	for(unsigned int i = 0; i < N_SEGMENTS_DRAW; ++i)
	{
		segment->render(x, dX, zOffset, camera, moduleRenderer, maxScreenY);
		
		if(segment->getIndex() == (unsigned int)(segments.size() - 1)) zOffset += length;

		x += dX;
		dX += segment->dX;

		segment = getSegment(segment->getIndex() + 1);
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

float ease(float a, float b, float percent)
{
	return a + (b - a) * ((-cos(percent * PI) / 2.0f) + 0.5f);
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