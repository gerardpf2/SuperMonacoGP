#ifndef _CAMERA_FOLLOW_
#define _CAMERA_FOLLOW_

#include "Camera.h"

class CameraFollow : public Camera
{
	public:

		CameraFollow(const WorldPosition* followPosition, const Road* road);

		virtual ~CameraFollow();

		virtual void update(float deltaTimeS) override;

	private:

		const WorldPosition* followPosition;
};

#endif