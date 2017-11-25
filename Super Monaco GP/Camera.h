#ifndef _CAMERA_
#define _CAMERA_

#include "Types.h"

class Road;

class Camera
{
	public:

		virtual ~Camera();

		const Road* getRoad() const;

		float getDepth() const;

		const WorldPosition* getPosition() const;

		bool isBehind(float z) const;

		void project(const WorldPosition& worldPosition, WindowPosition& windowPosition) const;

		void project(const WorldTrapezoid& worldTrapezoid, WindowTrapezoid& windowTrapezoid) const;

		virtual void update(float deltaTimeS);

	protected:

		Camera(const Road* road);

		void limitZ();

	protected:

		WorldPosition position;

	private:

		float depth;

		const Road* road = nullptr;
};

#endif