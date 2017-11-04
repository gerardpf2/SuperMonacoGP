#include "Road.h"

#include "Globals.h"
#include "Segment.h"

Road::Road(float length)
{
	float zNear = 0.0f;
	unsigned int nSegments = (unsigned int)ceilf(length / SEGMENT_LENGTH);

	segments.reserve(nSegments);

	for(unsigned int i = 0; i < nSegments; ++i)
	{
		float zFar = zNear + SEGMENT_LENGTH;

		segments.push_back(new Segment(zNear, zFar));

		zNear = zFar;
	}
}

Road::~Road()
{ }

const Segment* Road::getSegmentAtZ(float z) const
{
	return segments[(int)(z / SEGMENT_LENGTH) % segments.size()];
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