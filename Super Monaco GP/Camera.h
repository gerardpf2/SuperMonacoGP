#ifndef _CAMERA_
#define _CAMERA_

#include "Globals.h"

class Camera
{
	public:

		Camera(const Position3f& position, float fieldOfView);

		~Camera();

		void getPositionWorldToScreen(const Position3f& worldPosition, Position2i& screenPosition) const;

	public:
	// private:

		Position3f position;

		float fieldOfView;

		float depth;
};

#endif