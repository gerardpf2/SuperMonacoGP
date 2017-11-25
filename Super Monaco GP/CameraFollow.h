#ifndef _CAMERA_FOLLOW_
#define _CAMERA_FOLLOW_

#include "Camera.h"

class CameraFollow : public Camera
{
	public:

		CameraFollow(const Road* road, const WorldPosition* followPosition);

		virtual ~CameraFollow();

		const WorldPosition* getFollowPosition() const;

		virtual void update(float deltaTimeS) override;

	private:

		const WorldPosition* followPosition = nullptr;
};

#endif