#include "Road.h"

#include "Utils.h"
#include "Camera.h"
#include "Segment.h"
#include "GameObject.h"

Road::Road(const Document& document)
{
	// Length
	setLength(document["length"].GetFloat());

	// Hills
	addHills(document["hills"]);

	// Curves
	addCurves(document["curves"]);

	// RumbleColors
	setRumbleColors(document["rumbleColors"]);
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

void Road::render(const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	short maxScreenY = WINDOW_HEIGHT;

	float zOffset = 0.0f;
	float zBase = camera->getPosition()->third;

	Segment* baseSegment = getSegmentAtZ(zBase);

	float x = 0.0f;
	float dX = baseSegment->dX - baseSegment->dX * fmodf(zBase, SEGMENT_LENGTH) / SEGMENT_LENGTH;

	for(unsigned int i = 0; i < N_SEGMENTS_DRAW; ++i)
	{
		Segment* segment = getSegment(baseSegment->getIndex() + i);

		segment->xOffset = x;
		segment->zOffset = zOffset;
		segment->clipY = maxScreenY;

		segment->render(x, dX, zOffset, camera, moduleRenderer, maxScreenY);

		x += dX;
		dX += segment->dX;

		if(segment->getIndex() == (unsigned int)(segments.size() - 1)) zOffset += length;
	}

	// GameObjects

	for(int i = N_SEGMENTS_DRAW - 1; i >= 0; --i)
	{
		Segment* segment = getSegment(baseSegment->getIndex() + i);

		for(unsigned int i = 0; i < (unsigned int)segment->gameObjects.size(); ++i)
			segment->gameObjects[i]->render(segment->xOffset, segment->zOffset, camera, moduleRenderer, segment->clipY);
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

	for(int i = (int)rumbleColors.size() - 1; i >= 0; --i)
	{
		delete rumbleColors[i];
		rumbleColors[i] = nullptr;
	}

	rumbleColors.clear();
}

Segment* Road::getSegment(int index) const
{
	return segments[modI0ToL(index, segments.size())];
}

void Road::setLength(float length)
{
	if(length < ROAD_MIN_LENGTH)
		length = ROAD_MIN_LENGTH;

	this->length = length;

	unsigned int nSegments = (unsigned int)ceilf(this->length / SEGMENT_LENGTH);
	segments.reserve(nSegments);

	float currentZNear = 0.0f;

	for(unsigned int i = 0; i < nSegments; ++i)
	{
		Segment* segment = new Segment(i, currentZNear);

		segments.push_back(segment);
		currentZNear = segment->getZFar();
	}
}

void Road::addHills(const Value& value) const
{
	for(SizeType i = 0; i < value.Size(); ++i)
	{
		if(value[i]["defAngle"].GetBool())
		{
			float zStart = value[i]["zStart"].GetFloat();
			float length = value[i]["length"].GetFloat();
			float angle = value[i]["angle"].GetFloat();

			addHill(zStart, length, angle);
		}
		else
		{
			float zStart = value[i]["zStart"].GetFloat();
			float enterLength = value[i]["enterLength"].GetFloat();
			float holdLength = value[i]["holdLength"].GetFloat();
			float leaveLength = value[i]["leaveLength"].GetFloat();
			float val = value[i]["value"].GetFloat();

			addHill(zStart, enterLength, holdLength, leaveLength, val);
		}
	}
}

void Road::addHill(float zStart, float length, float angle) const
{
	float enterLength = length * 0.475f;
	float holdLength = length * 0.05f;
	float leaveLength = length * 0.475f;

	float value = (enterLength + holdLength / 2.0f) * tanf(degToRad(angle));

	addHill(zStart, enterLength, holdLength, leaveLength, value);
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

void Road::addHillEnter(unsigned int indexStart, unsigned int indexEnd, float enterLength, float value) const
{
	float c = 0.0f;
	float nSegments = enterLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		segment->yNear = getSegment((int)segment->getIndex() - 1)->yFar;
		segment->yFar = ease(0.0f, value, c / nSegments);

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addHillHold(unsigned int indexStart, unsigned int indexEnd, float holdLength, float value) const
{
	float c = 0.0f;
	float nSegments = holdLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		segment->yNear = getSegment((int)segment->getIndex() - 1)->yFar;
		segment->yFar = ease(segment->yNear, value, c / nSegments);

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addHillLeave(unsigned int indexStart, unsigned int indexEnd, float leaveLength, float value) const
{
	float c = 0.0f;
	float nSegments = leaveLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		segment->yNear = getSegment((int)segment->getIndex() - 1)->yFar;
		segment->yFar = ease(value, 0.0f, c / nSegments);

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addCurves(const Value& value) const
{
	for(SizeType i = 0; i < value.Size(); ++i)
	{
		if(value[i]["defAngle"].GetBool())
		{
			float zStart = value[i]["zStart"].GetFloat();
			float length = value[i]["length"].GetFloat();
			float angle = value[i]["angle"].GetFloat();

			addCurve(zStart, length, angle);
		}
		else
		{
			float zStart = value[i]["zStart"].GetFloat();
			float enterLength = value[i]["enterLength"].GetFloat();
			float holdLength = value[i]["holdLength"].GetFloat();
			float leaveLength = value[i]["leaveLength"].GetFloat();
			float val = value[i]["value"].GetFloat();

			addCurve(zStart, enterLength, holdLength, leaveLength, val);
		}
	}
}

void Road::addCurve(float zStart, float length, float angle) const
{
	float enterLength = length * 0.333f;
	float holdLength = length * 0.333f;
	float leaveLength = length * 0.333f;

	float value = length * sinf(degToRad(angle)) / powf(length / SEGMENT_LENGTH, 2.0f);

	addCurve(zStart, enterLength, holdLength, leaveLength, value);
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

void Road::addCurveEnter(unsigned int indexStart, unsigned int indexEnd, float enterLength, float value) const
{
	float c = 0.0f;
	float nSegments = enterLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++c)
	{
		getSegment(i)->dX = ease(0.0f, value, c / nSegments);

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addCurveHold(unsigned int indexStart, unsigned int indexEnd, float holdLength, float value) const
{
	for(unsigned int i = indexStart; i != indexEnd;)
	{
		getSegment(i)->dX = value;

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addCurveLeave(unsigned int indexStart, unsigned int indexEnd, float leaveLength, float value) const
{
	float c = 0.0f;
	float nSegments = leaveLength / SEGMENT_LENGTH;

	for(unsigned int i = indexStart; i != indexEnd; ++c)
	{
		getSegment(i)->dX = ease(value, 0.0f, c / nSegments);

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::setRumbleColors(const Value& value)
{
	unsigned int color0 = rgbaToUIint(value[0]["r"].GetUint(), value[0]["g"].GetUint(), value[0]["b"].GetUint(), value[0]["a"].GetUint());
	unsigned int color1 = rgbaToUIint(value[1]["r"].GetUint(), value[1]["g"].GetUint(), value[1]["b"].GetUint(), value[1]["a"].GetUint());
	unsigned int color2 = rgbaToUIint(value[2]["r"].GetUint(), value[2]["g"].GetUint(), value[2]["b"].GetUint(), value[2]["a"].GetUint());
	unsigned int color3 = rgbaToUIint(value[3]["r"].GetUint(), value[3]["g"].GetUint(), value[3]["b"].GetUint(), value[3]["a"].GetUint());
	unsigned int color4 = rgbaToUIint(value[4]["r"].GetUint(), value[4]["g"].GetUint(), value[4]["b"].GetUint(), value[4]["a"].GetUint());
	unsigned int color5 = rgbaToUIint(value[5]["r"].GetUint(), value[5]["g"].GetUint(), value[5]["b"].GetUint(), value[5]["a"].GetUint());
	unsigned int color6 = rgbaToUIint(value[6]["r"].GetUint(), value[6]["g"].GetUint(), value[6]["b"].GetUint(), value[6]["a"].GetUint());
	unsigned int color7 = rgbaToUIint(value[7]["r"].GetUint(), value[7]["g"].GetUint(), value[7]["b"].GetUint(), value[7]["a"].GetUint());
	unsigned int color8 = rgbaToUIint(value[8]["r"].GetUint(), value[8]["g"].GetUint(), value[8]["b"].GetUint(), value[8]["a"].GetUint());
	unsigned int color9 = rgbaToUIint(value[9]["r"].GetUint(), value[9]["g"].GetUint(), value[9]["b"].GetUint(), value[9]["a"].GetUint());

	unsigned int colors0[2]{ color0, color1 };
	unsigned int colors1[2]{ color2, color3 };
	unsigned int colors2[2]{ color4, color5 };
	unsigned int colors3[2]{ color6, color7 };
	unsigned int colors4[2]{ color8, color9 };

	unsigned int colorsIndex = 0;

	unsigned int nRumbleColors = (unsigned int)ceilf(length / ROAD_RUMBLE_HEIGHT);
	rumbleColors.reserve(nRumbleColors);

	for(unsigned int i = 0; i < nRumbleColors; ++i)
	{
		rumbleColors.push_back(new RumbleColors(colors0[colorsIndex], colors1[colorsIndex], colors2[colorsIndex], colors3[colorsIndex], colors4[colorsIndex]));
		colorsIndex = (colorsIndex + 1) % 2;
	}

	unsigned int rumbleColorsIndex = 0;
	unsigned int currentSegmentPerRumble = 0;
	unsigned int segmentsPerRumble = (unsigned int)(ROAD_RUMBLE_HEIGHT / SEGMENT_LENGTH);

	for(unsigned int i = 0; i < (unsigned int)segments.size(); ++i)
	{
		segments[i]->rumbleColors = rumbleColors[rumbleColorsIndex];

		if(++currentSegmentPerRumble == segmentsPerRumble)
		{
			++rumbleColorsIndex;
			currentSegmentPerRumble = 0;
		}
	}
}