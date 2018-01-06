#include "Road.h"

#include "Utils.h"
#include "Camera.h"
#include "Segment.h"
#include "GameObject.h"
#include "ModuleJson.h"
#include "ModuleTexture.h"

using namespace std;
using namespace rapidjson;

Road::Road()
{ }

Road::~Road()
{ }

void Road::load(const char* jsonPath, const ModuleJson* moduleJson, ModuleTexture* moduleTexture)
{
	assert(jsonPath);
	assert(moduleJson);

	unload(moduleTexture);

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

	// Background
	setBackgroundDefinition(jsonDocument["background"], moduleTexture);
}

void Road::unload(ModuleTexture* moduleTexture)
{
	for(int i = (int)segments.size() - 1; i >= 0; --i)
	{
		delete segments[i]; segments[i] = nullptr;
	}

	segments.clear();

	for(int i = (int)rumbleColors.size() - 1; i >= 0; --i)
	{
		delete rumbleColors[i]; rumbleColors[i] = nullptr;
	}

	rumbleColors.clear();

	for(int i = (int)gameObjectDefinitions.size() - 1; i >= 0; --i)
	{
		delete gameObjectDefinitions[i]; gameObjectDefinitions[i] = nullptr;
	}

	gameObjectDefinitions.clear();

	if(roadBackgroundDefinition)
	{
		assert(moduleTexture);

		moduleTexture->unload(roadBackgroundDefinition->textureGroupId);

		delete roadBackgroundDefinition; roadBackgroundDefinition = nullptr;
	}
}

float Road::getLength() const
{
	return length;
}

Segment* Road::getSegmentAtZ(float z) const
{
	return getSegment((int)(z / SEGMENT_LENGTH));
}

uint Road::getGameObjectsCount() const
{
	return (uint)gameObjectDefinitions.size();
}

const std::vector<RoadGameObjectDefinition*>* Road::getGameObjectDefinitions() const
{
	return &gameObjectDefinitions;
}

const RoadBackgroundDefinition* Road::getRoadBackgroundDefinition() const
{
	return roadBackgroundDefinition;
}

void Road::findGameObjectsFront(float z, float distance, list<const GameObject*>& gameObjects) const
{
	Segment* initialSegment = getSegmentAtZ(z);

	assert(initialSegment);

	int index = initialSegment->getIndex();
	int nSegments = (int)(distance / SEGMENT_LENGTH);

	for(int i = 0; i < nSegments; ++i)
	{
		Segment* segment = getSegment(index + i);

		assert(segment);
		assert(segment->getGameObjects());

		for(const GameObject* gameObject : *segment->getGameObjects())
			gameObjects.push_back(gameObject);
	}
}

void Road::render(const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	assert(camera);

	if(!camera->getForward()) renderMirror(camera, moduleRenderer);
	else
	{
		assert(camera->getPosition());

		float zRender0 = camera->getBasePositionZ();
		float zRender1 = camera->getBasePositionZ() - SEGMENT_LENGTH;

		float zFirst = mod0L(camera->getPosition()->z, length);
		float zBase0 = mod0L(zRender0, length);
		float zBase1 = mod0L(zRender1, length);
		float zLast = mod0L(zFirst + DRAW_DISTANCE, length);

		const Segment* first = getSegmentAtZ(zFirst);
		const Segment* base0 = getSegmentAtZ(zBase0);
		const Segment* base1 = getSegmentAtZ(zBase1);
		const Segment* last = getSegmentAtZ(zLast);

		// Render from player segment (included) to last segment (excluded)
		renderForward(zBase0, base0, last, zRender0, camera, moduleRenderer);
		// Render from player segment (excluded) to first segment (excluded)
		renderBackward(zBase1, base1, first, zRender1, camera, moduleRenderer);

		assert(first);

		for(int i = N_SEGMENTS_DRAW - 1; i > 0; --i)
		{
			Segment* segment = getSegment(first->getIndex() + i);

			assert(segment);
			assert(segment->getGameObjects());

			for(const GameObject* gameObject : *segment->getGameObjects())
			{
				assert(gameObject);

				gameObject->render(camera, moduleRenderer);
			}
		}
	}
}

void Road::renderMirror(const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	assert(camera);
	assert(camera->getPosition());

	float zRender0 = camera->getBasePositionZ() + SEGMENT_LENGTH;
	float zRender1 = camera->getBasePositionZ();

	float zFirst = mod0L(camera->getPosition()->z, length);
	float zBase0 = mod0L(zRender0, length);
	float zBase1 = mod0L(zRender1, length);
	float zLast = mod0L(zFirst - DRAW_DISTANCE, length);

	const Segment* first = getSegmentAtZ(zFirst);
	const Segment* base0 = getSegmentAtZ(zBase0);
	const Segment* base1 = getSegmentAtZ(zBase1);
	const Segment* last = getSegmentAtZ(zLast);

	// Render from player segment (excluded) to last segment (excluded)
	renderBackwardMirror(zBase1, base1, last, zRender1, camera, moduleRenderer);
	// Render from player segment (included) to first segment (excluded)
	renderForwardMirror(zBase0, base0, first, zRender0, camera, moduleRenderer);

	assert(last);

	for(int i = 1; i < N_SEGMENTS_DRAW; ++i)
	{
		Segment* segment = getSegment(last->getIndex() + i);

		assert(segment);
		assert(segment->getGameObjects());

		for(const GameObject* gameObject : *segment->getGameObjects())
		{
			assert(gameObject);

			gameObject->render(camera, moduleRenderer);
		}
	}
}

void Road::renderForward(float z, const Segment* first, const Segment* last, float renderZ, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	render(z, first, last, renderZ, 1.0f, -1.0f, false, 1, true, false, camera, moduleRenderer);
}

void Road::renderBackward(float z, const Segment* first, const Segment* last, float renderZ, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	render(z, first, last, renderZ, 0.0f, 1.0f, true, -1, false, false, camera, moduleRenderer);
}

void Road::renderForwardMirror(float z, const Segment* first, const Segment* last, float renderZ, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	render(z, first, last, renderZ, 1.0f, -1.0f, true, 1, false, true, camera, moduleRenderer);
}

void Road::renderBackwardMirror(float z, const Segment* first, const Segment* last, float renderZ, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	render(z, first, last, renderZ, 0.0f, 1.0f, false, -1, true, true, camera, moduleRenderer);
}

void Road::render(float z, const Segment* first, const Segment* last, float renderZ, float initialCurveMultiplier, float initialCurveDXMultiplier, bool invertZNearZFar, int increment, bool clip, bool mirror, const Camera* camera, const ModuleRenderer* moduleRenderer) const
{
	assert(first);

	short maxWindowY = WINDOW_HEIGHT;
	float accZOffset = -z + first->getZNear();
	Segment* current = getSegment(first->getIndex());

	assert(current);

	float x = 0.0f;
	float dX = initialCurveMultiplier * current->getCurve() + initialCurveDXMultiplier * current->getCurve() * interpolate01(z, current->getZNear(), current->getZFar());

	while(current != last)
	{
		assert(current);

		current->setXOffsetNear(invertZNearZFar ? x + dX : x);
		current->setXOffsetFar(invertZNearZFar ? x : x + dX);

		x += dX;
		dX += current->getCurve();

		float currentZOffset = -current->getZNear() + accZOffset;
		accZOffset += increment * SEGMENT_LENGTH;

		current->setZOffset(renderZ + currentZOffset);
		current->setClipY(maxWindowY);

		current->render(camera, moduleRenderer, maxWindowY, clip, mirror);

		current = getSegment(current->getIndex() + increment);
	}
}

Segment* Road::getSegment(int index) const
{
	index = mod0L(index, (int)segments.size());

	assert((int)segments.size() > index);

	return segments[index];
}

void Road::setLength(float length)
{
	if(length < ROAD_MIN_LENGTH)
		length = ROAD_MIN_LENGTH;

	this->length = length;

	uint nSegments = (uint)ceilf(this->length / SEGMENT_LENGTH);
	segments.reserve(nSegments);

	for(uint i = 0; i < nSegments; ++i)
		segments.push_back(new Segment(i));
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

	Segment* segmentEnter = getSegmentAtZ(zEnter);
	Segment* segmentHold = getSegmentAtZ(zHold);
	Segment* segmentLeave = getSegmentAtZ(zLeave);
	Segment* segmentExit = getSegmentAtZ(zExit);

	assert(segmentEnter);
	assert(segmentHold);
	assert(segmentLeave);
	assert(segmentExit);

	uint indexEnter = segmentEnter->getIndex();
	uint indexHold = segmentHold->getIndex();
	uint indexLeave = segmentLeave->getIndex();
	uint indexExit = segmentExit->getIndex();

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

		assert(segment);

		Segment* previousSegment = getSegment((int)segment->getIndex() - 1);

		assert(previousSegment);

		segment->setYNear(previousSegment->getYFar());
		segment->setYFar(ease(0.0f, value, c / nSegments));

		Segment* nextSegment = getSegment(i + 1);

		assert(nextSegment);

		i = nextSegment->getIndex();
	}
}

void Road::addHillHold(uint indexStart, uint indexEnd, float holdLength, float value) const
{
	float c = 0.0f;
	float nSegments = holdLength / SEGMENT_LENGTH;

	for(uint i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		assert(segment);

		Segment* previousSegment = getSegment((int)segment->getIndex() - 1);

		assert(previousSegment);

		segment->setYNear(previousSegment->getYFar());
		segment->setYFar(ease(segment->getYNear(), value, c / nSegments));

		Segment* nextSegment = getSegment(i + 1);

		assert(nextSegment);

		i = nextSegment->getIndex();
	}
}

void Road::addHillLeave(uint indexStart, uint indexEnd, float leaveLength, float value) const
{
	float c = 0.0f;
	float nSegments = leaveLength / SEGMENT_LENGTH;

	for(uint i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		assert(segment);

		Segment* previousSegment = getSegment((int)segment->getIndex() - 1);

		assert(previousSegment);

		segment->setYNear(previousSegment->getYFar());
		segment->setYFar(ease(value, 0.0f, c / nSegments));

		Segment* nextSegment = getSegment(i + 1);

		assert(nextSegment);

		i = nextSegment->getIndex();
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

	Segment* segmentEnter = getSegmentAtZ(zEnter);
	Segment* segmentHold = getSegmentAtZ(zHold);
	Segment* segmentLeave = getSegmentAtZ(zLeave);
	Segment* segmentExit = getSegmentAtZ(zExit);

	assert(segmentEnter);
	assert(segmentHold);
	assert(segmentLeave);
	assert(segmentExit);

	uint indexEnter = segmentEnter->getIndex();
	uint indexHold = segmentHold->getIndex();
	uint indexLeave = segmentLeave->getIndex();
	uint indexExit = segmentExit->getIndex();

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
		Segment* segment = getSegment(i);

		assert(segment);

		segment->setCurve(ease(0.0f, value, c / nSegments));

		Segment* nextSegment = getSegment(i + 1);

		assert(nextSegment);

		i = nextSegment->getIndex();
	}
}

void Road::addCurveHold(uint indexStart, uint indexEnd, float holdLength, float value) const
{
	for(uint i = indexStart; i != indexEnd;)
	{
		Segment* segment = getSegment(i);

		assert(segment);

		segment->setCurve(value);

		Segment* nextSegment = getSegment(i + 1);

		assert(nextSegment);

		i = nextSegment->getIndex();
	}
}

void Road::addCurveLeave(uint indexStart, uint indexEnd, float leaveLength, float value) const
{
	float c = 0.0f;
	float nSegments = leaveLength / SEGMENT_LENGTH;

	for(uint i = indexStart; i != indexEnd; ++c)
	{
		Segment* segment = getSegment(i);

		assert(segment);

		segment->setCurve(ease(value, 0.0f, c / nSegments));

		Segment* nextSegment = getSegment(i + 1);

		assert(nextSegment);

		i = nextSegment->getIndex();
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

	const int nColors = 2;

	uint colors0[nColors]{ color0, color1 };
	uint colors1[nColors]{ color2, color3 };
	uint colors2[nColors]{ color4, color5 };
	uint colors3[nColors]{ color6, color7 };
	uint colors4[nColors]{ color8, color9 };

	uint colorsIndex = 0;

	uint nRumbleColors = (uint)ceilf(length / ROAD_RUMBLE_HEIGHT);
	rumbleColors.reserve(nRumbleColors);

	for(uint i = 0; i < nRumbleColors; ++i)
	{
		assert(nColors > colorsIndex);

		rumbleColors.push_back(new RumbleColors{ colors0[colorsIndex], colors1[colorsIndex], colors2[colorsIndex], colors3[colorsIndex], colors4[colorsIndex] });
		colorsIndex = (colorsIndex + 1) % nColors;
	}

	if(!rumbleColors.empty())
	{
		assert(!rumbleColors.empty());

		RumbleColors* lastRumbleColors = rumbleColors[rumbleColors.size() - 1];

		assert(lastRumbleColors);

		lastRumbleColors->b = lastRumbleColors->c = lastRumbleColors->d = lastRumbleColors->e = color3;
	}

	uint rumbleColorsIndex = 0;
	uint currentSegmentPerRumble = 0;
	uint segmentsPerRumble = (uint)(ROAD_RUMBLE_HEIGHT / SEGMENT_LENGTH);

	for(Segment* segment : segments)
	{
		assert(segment);
		assert(rumbleColors.size() > rumbleColorsIndex);

		segment->setRumbleColors(rumbleColors[rumbleColorsIndex]);

		if(++currentSegmentPerRumble == segmentsPerRumble)
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
		// id, offsetX, initialPosition, incPosition, n

		uint id = value[i]["id"].GetUint();
		float offsetX = value[i]["offsetX"].GetFloat();
		const Value& initialPositionJson = value[i]["initialPosition"];
		const Value& incPositionJson = value[i]["incPosition"];
		uint n = value[i]["n"].GetUint();

		// initial x, y, z

		float x0 = initialPositionJson["x"].GetFloat();
		float y0 = initialPositionJson["y"].GetFloat();
		float z0 = initialPositionJson["z"].GetFloat();

		// inc x, y, z

		float x1 = incPositionJson["x"].GetFloat();
		float y1 = incPositionJson["y"].GetFloat();
		float z1 = incPositionJson["z"].GetFloat();

		for(uint i = 0; i < n; ++i)
		{
			gameObjectDefinitions.push_back(new RoadGameObjectDefinition{ id, offsetX, WorldPosition{ x0, y0, z0 } });

			x0 += x1;
			y0 += y1;
			z0 += z1;
		}
	}
}

void Road::setBackgroundDefinition(const rapidjson::Value& value, ModuleTexture* moduleTexture)
{
	assert(moduleTexture);

	const char* textureGroupPath = value["textureGroupPath"].GetString();
	uint textureId = value["textureId"].GetUint();
	uint textureSkyId = value["textureSkyId"].GetUint();
	uint textureGroundId = value["textureGroundId"].GetUint();

	roadBackgroundDefinition = new RoadBackgroundDefinition;

	roadBackgroundDefinition->textureGroupId = moduleTexture->load(textureGroupPath);
	roadBackgroundDefinition->textureId = textureId;
	roadBackgroundDefinition->textureSkyId = textureSkyId;
	roadBackgroundDefinition->textureGroundId = textureGroundId;
}