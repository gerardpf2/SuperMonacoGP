#ifndef _ROAD_
#define _ROAD_

#include <vector>
#include "Types.h"
#include "rapidjson/document.h"

class Camera;
class Segment;
class ModuleJson;
class ModuleRenderer;

class Road
{
	public:

		Road();

		virtual ~Road();

		void load(const char* jsonPath, const ModuleJson* moduleJson);

		void unload();

		float getLength() const;

		Segment* getSegmentAtZ(float z) const;

		const std::vector<RoadGameObjectDefinition*>* getGameObjectDefinitions() const;

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

	private:

		void render(float z, Segment* first, const Segment* last, const Segment* limit, int multiplier, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

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

		void addGameObjectDefinitions(const rapidjson::Value& value);

	private:

		float length;

		std::vector<Segment*> segments;

		std::vector<RumbleColors*> rumbleColors;

		std::vector<RoadGameObjectDefinition*> gameObjectDefinitions;
};

#endif