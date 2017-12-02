#include "Road.h"

#include "Utils.h"
#include "Camera.h"
#include "Segment.h"
#include "GameObject.h"
#include "ModuleJson.h"

using namespace rapidjson;

Road::Road()
{ }

Road::~Road()
{ }

void Road::load(const char* jsonPath, const ModuleJson* moduleJson)
{
	unload();

	Document jsonDocument;
	moduleJson->read(jsonPath, jsonDocument);

	// Length
	setLength(jsonDocument["length"].GetFloat());

	// Hills
	addHills(jsonDocument["hills"]);

	// Curves
	addCurves(jsonDocument["curves"]);

	// RumbleColors
	setRumbleColors(jsonDocument["rumbleColors"]);

	// GameObjectDefinitions
	addGameObjectDefinitions(jsonDocument["gameObjectDefinitions"]);
}

void Road::unload()
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

	for(int i = (int)gameObjectDefinitions.size() - 1; i >= 0; --i)
	{
		delete gameObjectDefinitions[i];
		gameObjectDefinitions[i] = nullptr;
	}

	gameObjectDefinitions.clear();
}

float Road::getLength() const
{
	return length;
}

Segment* Road::getSegmentAtZ(float z) const
{
	return getSegment((int)(z / SEGMENT_LENGTH));
}

const std::vector<RoadGameObjectDefinition*>* Road::getGameObjectDefinitions() const
{
	return &gameObjectDefinitions;
}

void Road::render(const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	short maxWindowY = WINDOW_HEIGHT;

	float zOffset = 0.0f;
	float zBase = camera->getBasePositionZ();

	Segment* baseSegment = getSegmentAtZ(zBase);

	float x = 0.0f;
	float dX = baseSegment->getCurve() - baseSegment->getCurve() * fmodf(zBase, SEGMENT_LENGTH) / SEGMENT_LENGTH;

	for(uint i = 0; i < N_SEGMENTS_DRAW; ++i)
	{
		Segment* segment = getSegment(baseSegment->getIndex() + i);

		segment->setXOffsetNear(x);
		segment->setXOffsetFar(x + dX);
		segment->setZOffset(zOffset);
		segment->setClipY(maxWindowY);

		x += dX;
		dX += segment->getCurve();

		segment->render(camera, moduleRenderer, maxWindowY);

		if(segment->getIndex() == (uint)(segments.size() - 1)) zOffset += length;
	}

	// GameObjects

	for(int i = N_SEGMENTS_DRAW - 1; i >= 0; --i)
	{
		Segment* segment = getSegment(baseSegment->getIndex() + i);

		for(const GameObject* gameObject : *segment->getGameObjects())
			gameObject->render(camera, moduleRenderer);
	}
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

	uint nSegments = (uint)ceilf(this->length / SEGMENT_LENGTH);
	segments.reserve(nSegments);

	float currentZNear = 0.0f;

	for(uint i = 0; i < nSegments; ++i)
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
	float enterLength = length * ROAD_HILL_ENTER_PERCENT;
	float holdLength = length * ROAD_HILL_HOLD_PERCENT;
	float leaveLength = length * ROAD_HILL_LEAVE_PERCENT;

	float value = (enterLength + holdLength / 2.0f) * tanf(degToRad(angle));

	addHill(zStart, enterLength, holdLength, leaveLength, value);
}

void Road::addHill(float zStart, float enterLength, float holdLength, float leaveLength, float value) const
{
	float zEnter = zStart;
	float zHold = zEnter + enterLength;
	float zLeave = zHold + holdLength;
	float zExit = zLeave + leaveLength;

	uint indexEnter = getSegmentAtZ(zEnter)->getIndex();
	uint indexHold = getSegmentAtZ(zHold)->getIndex();
	uint indexLeave = getSegmentAtZ(zLeave)->getIndex();
	uint indexExit = getSegmentAtZ(zExit)->getIndex();

	addHillEnter(indexEnter, indexHold, enterLength, value);
	addHillHold(indexHold, indexLeave, holdLength, value);
	addHillLeave(indexLeave, indexExit, leaveLength, value);
}

void Road::addHillEnter(uint indexStart, uint indexEnd, float enterLength, float value) const
{
	float c = 0.0f;
	float nSegments = enterLength / SEGMENT_LENGTH;

	for(uint i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		segment->setYNear(getSegment((int)segment->getIndex() - 1)->getYFar());
		segment->setYFar(ease(0.0f, value, c / nSegments));

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addHillHold(uint indexStart, uint indexEnd, float holdLength, float value) const
{
	float c = 0.0f;
	float nSegments = holdLength / SEGMENT_LENGTH;

	for(uint i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		segment->setYNear(getSegment((int)segment->getIndex() - 1)->getYFar());
		segment->setYFar(ease(segment->getYNear(), value, c / nSegments));

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addHillLeave(uint indexStart, uint indexEnd, float leaveLength, float value) const
{
	float c = 0.0f;
	float nSegments = leaveLength / SEGMENT_LENGTH;

	for(uint i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		segment->setYNear(getSegment((int)segment->getIndex() - 1)->getYFar());
		segment->setYFar(ease(value, 0.0f, c / nSegments));

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
	float enterLength = length * ROAD_CURVE_ENTER_PERCENT;
	float holdLength = length * ROAD_CURVE_HOLD_PERCENT;
	float leaveLength = length * ROAD_CURVE_LEAVE_PERCENT;

	float value = length * sinf(degToRad(angle)) / powf(length / SEGMENT_LENGTH, 2.0f);

	addCurve(zStart, enterLength, holdLength, leaveLength, value);
}

void Road::addCurve(float zStart, float enterLength, float holdLength, float leaveLength, float value) const
{
	float zEnter = zStart;
	float zHold = zEnter + enterLength;
	float zLeave = zHold + holdLength;
	float zExit = zLeave + leaveLength;

	uint indexEnter = getSegmentAtZ(zEnter)->getIndex();
	uint indexHold = getSegmentAtZ(zHold)->getIndex();
	uint indexLeave = getSegmentAtZ(zLeave)->getIndex();
	uint indexExit = getSegmentAtZ(zExit)->getIndex();

	addCurveEnter(indexEnter, indexHold, enterLength, value);
	addCurveHold(indexHold, indexLeave, holdLength, value);
	addCurveLeave(indexLeave, indexExit, leaveLength, value);
}

void Road::addCurveEnter(uint indexStart, uint indexEnd, float enterLength, float value) const
{
	float c = 0.0f;
	float nSegments = enterLength / SEGMENT_LENGTH;

	for(uint i = indexStart; i != indexEnd; ++c)
	{
		getSegment(i)->setCurve(ease(0.0f, value, c / nSegments));

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addCurveHold(uint indexStart, uint indexEnd, float holdLength, float value) const
{
	for(uint i = indexStart; i != indexEnd;)
	{
		getSegment(i)->setCurve(value);

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::addCurveLeave(uint indexStart, uint indexEnd, float leaveLength, float value) const
{
	float c = 0.0f;
	float nSegments = leaveLength / SEGMENT_LENGTH;

	for(uint i = indexStart; i != indexEnd; ++c)
	{
		getSegment(i)->setCurve(ease(value, 0.0f, c / nSegments));

		i = getSegment(i + 1)->getIndex();
	}
}

void Road::setRumbleColors(const Value& value)
{
	uint color0 = RGBAToUint(value[0]["r"].GetUint(), value[0]["g"].GetUint(), value[0]["b"].GetUint(), value[0]["a"].GetUint());
	uint color1 = RGBAToUint(value[1]["r"].GetUint(), value[1]["g"].GetUint(), value[1]["b"].GetUint(), value[1]["a"].GetUint());
	uint color2 = RGBAToUint(value[2]["r"].GetUint(), value[2]["g"].GetUint(), value[2]["b"].GetUint(), value[2]["a"].GetUint());
	uint color3 = RGBAToUint(value[3]["r"].GetUint(), value[3]["g"].GetUint(), value[3]["b"].GetUint(), value[3]["a"].GetUint());
	uint color4 = RGBAToUint(value[4]["r"].GetUint(), value[4]["g"].GetUint(), value[4]["b"].GetUint(), value[4]["a"].GetUint());
	uint color5 = RGBAToUint(value[5]["r"].GetUint(), value[5]["g"].GetUint(), value[5]["b"].GetUint(), value[5]["a"].GetUint());
	uint color6 = RGBAToUint(value[6]["r"].GetUint(), value[6]["g"].GetUint(), value[6]["b"].GetUint(), value[6]["a"].GetUint());
	uint color7 = RGBAToUint(value[7]["r"].GetUint(), value[7]["g"].GetUint(), value[7]["b"].GetUint(), value[7]["a"].GetUint());
	uint color8 = RGBAToUint(value[8]["r"].GetUint(), value[8]["g"].GetUint(), value[8]["b"].GetUint(), value[8]["a"].GetUint());
	uint color9 = RGBAToUint(value[9]["r"].GetUint(), value[9]["g"].GetUint(), value[9]["b"].GetUint(), value[9]["a"].GetUint());

	uint colors0[2]{ color0, color1 };
	uint colors1[2]{ color2, color3 };
	uint colors2[2]{ color4, color5 };
	uint colors3[2]{ color6, color7 };
	uint colors4[2]{ color8, color9 };

	uint colorsIndex = 0;

	uint nRumbleColors = (uint)ceilf(length / ROAD_RUMBLE_HEIGHT);
	rumbleColors.reserve(nRumbleColors);

	for(uint i = 0; i < nRumbleColors; ++i)
	{
		rumbleColors.push_back(new RumbleColors{ colors0[colorsIndex], colors1[colorsIndex], colors2[colorsIndex], colors3[colorsIndex], colors4[colorsIndex] });
		colorsIndex = (colorsIndex + 1) % 2;
	}

	uint rumbleColorsIndex = 0;
	uint currentSegmentPerRumble = 0;
	uint segmentsPerRumble = (uint)(ROAD_RUMBLE_HEIGHT / SEGMENT_LENGTH);

	for(uint i = 0; i < (uint)segments.size(); ++i)
	{
		segments[i]->setRumbleColors(rumbleColors[rumbleColorsIndex]);

		if(currentSegmentPerRumble++ == segmentsPerRumble)
		{
			++rumbleColorsIndex;
			currentSegmentPerRumble = 0;
		}
	}
}

void Road::addGameObjectDefinitions(const rapidjson::Value& value)
{
	gameObjectDefinitions.reserve(value.Size());

	for(SizeType i = 0; i < value.Size(); ++i)
	{
		// id, offsetX, position

		uint id = value[i]["id"].GetUint();
		float offsetX = value[i]["offsetX"].GetFloat();
		const Value& positionJson = value[i]["position"];

		// x, y, z

		float x = positionJson["x"].GetFloat();
		float y = positionJson["y"].GetFloat();
		float z = positionJson["z"].GetFloat();

		gameObjectDefinitions.push_back(new RoadGameObjectDefinition{ id, offsetX, WorldPosition{ x, y, z } });
	}
}