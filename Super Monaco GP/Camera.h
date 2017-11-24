#ifndef _CAMERA_
#define _CAMERA_

#include "Types.h"

class Road;

class Camera
{
	public:

		virtual ~Camera();

		float getDepth() const;

		const WorldPosition* getPosition() const;

		bool isBehind(float z) const;

		virtual void update(float deltaTimeS);

		void project(const WorldPosition& worldPosition, WindowPosition& windowPosition) const;

		void project(const WorldTrapezoid& worldTrapezoid, WindowTrapezoid& windowTrapezoid) const;

	protected:

		Camera(const Road* road);

		void limitZ();

	protected:

		float depth;

		WorldPosition position;

		const Road* road = nullptr;
};

#endif