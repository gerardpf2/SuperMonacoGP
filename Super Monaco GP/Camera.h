#ifndef _CAMERA_
#define _CAMERA_

#include "Types.h"

class Road;

class Camera
{
	public:

		virtual ~Camera();

		bool getForward() const;

		const Road* getRoad() const;

		float getDepth() const;

		const WorldPosition* getPosition() const;

		float getOffsetZ() const;

		float getBasePositionZ() const;

		bool isBehind(float z) const;

		void project(const WorldPosition& worldPosition, WindowPosition& windowPosition) const;

		void project(const WorldTrapezoid& worldTrapezoid, WindowTrapezoid& windowTrapezoid) const;

		virtual void update(float deltaTimeS);

	protected:

		Camera(bool forward, const Road* road);

		void limitZ();

	protected:

		WorldPosition position;

	private:

		bool forward;

		const Road* road = nullptr;

		float depth;
};

#endif