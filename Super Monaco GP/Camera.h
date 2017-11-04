#ifndef _CAMERA_
#define _CAMERA_

#include "Globals.h"

class Camera
{
	public:

		Camera();

		virtual ~Camera();

		const Position3f* getPosition() const;

		void moveY(float incY);

		void moveXZ(float incX, float incZ);

		void follow(const Position3f& position);

		void getPositionWorldToScreen(const Position3f& worldPosition, Position2i& screenPosition) const;

	private:

		float depth;

		Position3f position;
};

#endif