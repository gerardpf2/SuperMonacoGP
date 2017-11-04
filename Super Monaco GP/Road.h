#ifndef _ROAD_
#define _ROAD_

#include <vector>

using namespace std;

class Segment;

class Road
{
	public:

		Road(float length);

		virtual ~Road();

		const Segment* getSegmentAtZ(float z) const;

		void clear();

	private:

		vector<Segment*> segments;
};

#endif