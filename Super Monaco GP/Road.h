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

		void addCurve(float zStart, float enterLength, float holdLength, float leaveLength, float value) const;

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void clear();

	private:

		Segment* getSegment(int index) const;

		Segment* getSegmentAtZ(float z) const;

		void addCurveEnter(unsigned int indexStart, unsigned int indexEnd, float value) const;

		void addCurveHold(unsigned int indexStart, unsigned int indexEnd, float value) const;

		void addCurveLeave(unsigned int indexStart, unsigned int indexEnd, float value) const;

	private:

		float length;

		vector<Segment*> segments;
};

#endif