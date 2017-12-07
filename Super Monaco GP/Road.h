#ifndef _ROAD_
#define _ROAD_

#include <vector>
#include "Types.h"
#include "rapidjson/document.h"

class Camera;
class Segment;
class Background;
class ModuleJson;
class ModuleTexture;
class ModuleRenderer;

class Road
{
	public:

		Road();

		virtual ~Road();

		void load(const char* jsonPath, const ModuleJson* moduleJson, ModuleTexture* moduleTexture);

		void unload(ModuleTexture* moduleTexture);

		float getLength() const;

		Segment* getSegmentAtZ(float z) const;

		const std::vector<RoadGameObjectDefinition*>* getGameObjectDefinitions() const;

		void update(const Camera* camera, float deltaTimeS) const;

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

	private:

		void renderMirror(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void renderForward(float z, const Segment* first, const Segment* last, float renderZ, const Camera* camera, const ModuleRenderer* moduleRenderer) const;
		
		void renderBackward(float z, const Segment* first, const Segment* last, float renderZ, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void renderForwardMirror(float z, const Segment* first, const Segment* last, float renderZ, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void renderBackwardMirror(float z, const Segment* first, const Segment* last, float renderZ, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void render(float z, const Segment* first, const Segment* last, float renderZ, float initialCurveMultiplier, float initialCurveDXMultiplier, bool invertZNearZFar, int increment, bool clip, bool mirror, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		Segment* getSegment(int index) const;

		void setLength(float length);

		void addHills(const rapidjson::Value& value) const;

		void addHill(float zStart, float length, float angle) const;

		void addHill(float zStart, float enterLength, float holdLength, float leaveLength, float value) const;

		void addHillEnter(uint indexStart, uint indexEnd, float enterLength, float value) const;

		void addHillHold(uint indexStart, uint indexEnd, float holdLength, float value) const;

		void addHillLeave(uint indexStart, uint indexEnd, float leaveLength, float value) const;

		void addCurves(const rapidjson::Value& value) const;

		void addCurve(float zStart, float length, float angle) const;

		void addCurve(float zStart, float enterLength, float holdLength, float leaveLength, float value) const;

		void addCurveEnter(uint indexStart, uint indexEnd, float enterLength, float value) const;

		void addCurveHold(uint indexStart, uint indexEnd, float holdLength, float value) const;

		void addCurveLeave(uint indexStart, uint indexEnd, float leaveLength, float value) const;

		void setRumbleColors(const rapidjson::Value& value);

		void setBackground(const rapidjson::Value& value, ModuleTexture* moduleTexture);

		void addGameObjectDefinitions(const rapidjson::Value& value);

	private:

		float length;

		std::vector<Segment*> segments;

		std::vector<RumbleColors*> rumbleColors;

		std::vector<RoadGameObjectDefinition*> gameObjectDefinitions;

		uint backroundTextureGroupId = -1;

		Background* background = nullptr;
};

#endif