#ifndef _ROAD_
#define _ROAD_

#include <vector>
#include "Globals.h"

using namespace std;

class Camera;
class Segment;
class ModuleRenderer;

class Road
{
	public:

		Road(float length = ROAD_MIN_LENGTH);

		virtual ~Road();

		float getLength() const;

		Segment* getSegmentAtZ(float z) const;

		void addHill(float zStart, float enterLength, float holdLength, float leaveLength, float value) const;

		void addCurve(float zStart, float enterLength, float holdLength, float leaveLength, float value) const;

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void clear();

	private:

		Segment* getSegment(int index) const;

		void addHillEnter(unsigned int indexStart, unsigned int indexEnd, float enterLength, float value) const;

		void addHillHold(unsigned int indexStart, unsigned int indexEnd, float holdLength, float value) const;

		void addHillLeave(unsigned int indexStart, unsigned int indexEnd, float leaveLength, float value) const;

		void addCurveEnter(unsigned int indexStart, unsigned int indexEnd, float enterLength, float value) const;

		void addCurveHold(unsigned int indexStart, unsigned int indexEnd, float holdLength, float value) const;

		void addCurveLeave(unsigned int indexStart, unsigned int indexEnd, float leaveLength, float value) const;

	private:

		float length;

		vector<Segment*> segments;
};

#endif