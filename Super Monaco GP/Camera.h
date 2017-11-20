#ifndef _CAMERA_
#define _CAMERA_

#include "Types.h"

class Road;

class Camera
{
	public:

		virtual ~Camera();

		const WorldPosition* getPosition() const;

		virtual void update(float deltaTimeS);

		// float getBaseZ() const;

		bool getIsBehind(float z) const;

		void project(const WorldPosition& worldPosition, ScreenPosition& screenPosition) const;

		void project(const WorldPosition& worldPositionBL, const WorldPosition& worldPositionBR, const WorldPosition& worldPositionUR, const WorldPosition& worldPositionUL, ScreenPosition& screenPositionBL, ScreenPosition& screenPositionBR, ScreenPosition& screenPositionUR, ScreenPosition& screenPositionUL) const;

	protected:

		Camera(const Road* road);

		void updateLimitZRoad();

	protected:

		float depth;

		WorldPosition position;

	private:

		const Road* road = nullptr;
};

#endif