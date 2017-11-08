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

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void clear();

	private:

		const Segment* getSegmentAtZ(float z) const;

	private:

		float length;

		vector<Segment*> segments;
};

#endif