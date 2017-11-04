#ifndef _SEGMENT_
#define _SEGMENT_

class Segment
{
	public:

		Segment(float zNear, float zFar);

		virtual ~Segment();

	private:

		float zNear, zFar;
};

#endif