#ifndef _CAMERA_
#define _CAMERA_

#include "Globals.h"

class Road;

class Camera
{
	public:

		virtual ~Camera();

		const Position3f* getPosition() const;

		virtual void update(float deltaTimeS);

		// float getBaseZ() const;

		bool getIsBehind(float z) const;

		void getPositionWorldToScreen(const Position3f& worldPosition, Position2s& screenPosition) const;

	protected:

		Camera(const Road* road);

		void updateLimitZRoad();

	protected:

		Position3f position;

	private:

		float depth;

		const Road* road;
};

#endif