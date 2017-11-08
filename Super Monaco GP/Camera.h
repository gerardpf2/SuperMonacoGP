#ifndef _CAMERA_
#define _CAMERA_

#include "Globals.h"

class Road;

class Camera
{
	public:

		virtual ~Camera();

		virtual void update(float deltaTimeS);

		float getBaseZ() const;

		bool getIsBehind(float z) const;

		void getPositionWorldToScreen(const Position3f& worldPosition, Position2s& screenPosition) const;

	protected:

		Camera(const Road* road);

		void updateLimitZRoad();

	protected:

		float depth;

		Position3f position;

	private:

		const Road* road;
};

#endif