#ifndef _CAMERA_FOLLOW_
#define _CAMERA_FOLLOW_

#include "Camera.h"

class CameraFollow : public Camera
{
	public:

		CameraFollow(bool forward, const Road* road, const WorldPosition* followPosition, float projectionY0 = 1.5f, float projectionY1 = 3.0f, const WorldPosition& offsetPosition = WorldPosition{ 0.0f, 0.0f, 0.0f });

		virtual ~CameraFollow();

		const WorldPosition* getFollowPosition() const;

		virtual void update(float deltaTimeS) override;

	private:

		const WorldPosition* followPosition = nullptr; // Constructor

		WorldPosition offsetPosition; // Constructor
};

#endif