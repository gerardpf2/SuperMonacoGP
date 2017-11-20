#ifndef _ROAD_
#define _ROAD_

#include <vector>
#include "Types.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

class Camera;
class Segment;
class ModuleRenderer;

class Road
{
	public:

		Road(const Document& document);

		virtual ~Road();

		float getLength() const;

		Segment* getSegmentAtZ(float z) const;

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void clear();

	private:

		Segment* getSegment(int index) const;

		void setLength(float length);

		void addHills(const Value& value) const;

		void addHill(float zStart, float length, float angle) const;

		void addHill(float zStart, float enterLength, float holdLength, float leaveLength, float value) const;

		void addHillEnter(unsigned int indexStart, unsigned int indexEnd, float enterLength, float value) const;

		void addHillHold(unsigned int indexStart, unsigned int indexEnd, float holdLength, float value) const;

		void addHillLeave(unsigned int indexStart, unsigned int indexEnd, float leaveLength, float value) const;

		void addCurves(const Value& value) const;

		void addCurve(float zStart, float length, float angle) const;

		void addCurve(float zStart, float enterLength, float holdLength, float leaveLength, float value) const;

		void addCurveEnter(unsigned int indexStart, unsigned int indexEnd, float enterLength, float value) const;

		void addCurveHold(unsigned int indexStart, unsigned int indexEnd, float holdLength, float value) const;

		void addCurveLeave(unsigned int indexStart, unsigned int indexEnd, float leaveLength, float value) const;

		void setRumbleColors(const Value& value);

	private:

		float length;

		vector<Segment*> segments;

		vector<RumbleColors*> rumbleColors;
};

#endif