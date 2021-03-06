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

		float getProjectionY0() const;
		
		float getProjectionY1() const;

		float getDepth() const;

		const WorldPosition* getPosition() const;

		float getOffsetZ() const;

		float getBasePositionZ() const;

		bool isBehind(float z) const;

		void project(const WorldPosition& worldPosition, WindowPosition& windowPosition) const;

		void project(const WorldTrapezoid& worldTrapezoid, WindowTrapezoid& windowTrapezoid) const;

		virtual void update(float deltaTimeS);

	protected:

		Camera(bool forward, const Road* road, float projectionY0 = 1.5f, float projectionY1 = 3.0f);

		void limitZ();

	protected:

		WorldPosition position;

	private:

		bool forward; // Constructor

		const Road* road = nullptr; // Constructor

		float projectionY0, projectionY1; // Constructor

		float depth = 1.0f;
};

#endif