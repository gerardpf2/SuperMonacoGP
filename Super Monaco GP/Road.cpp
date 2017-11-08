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
		Segment* segment = new Segment(currentZNear, colors[(colorIndex++) % 2]);

		segments.push_back(segment);
		currentZNear = segment->getZFar();
	}

	segments[0]->color = BLUE;
	segments[segments.size() - 1]->color = GREEN;
}

Road::~Road()
{ }

float Road::getLength() const
{
	return length;
}

void Road::render(const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	float zOffset = 0.0f;
	float zBase = camera->getBaseZ();
	short maxScreenY = WINDOW_HEIGHT;

	for(unsigned int i = 0; i < N_SEGMENTS_DRAW; ++i)
	{
		const Segment* segment = getSegmentAtZ(zBase);
		
		segment->render(zOffset, camera, moduleRenderer, maxScreenY);
		
		if(segment == segments[segments.size() - 1]) zOffset += length;

		zBase += SEGMENT_LENGTH;
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

const Segment* Road::getSegmentAtZ(float z) const
{
	return segments[(int)(z / SEGMENT_LENGTH) % segments.size()];
}