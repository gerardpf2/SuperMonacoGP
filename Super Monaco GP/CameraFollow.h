#ifndef _CAMERA_FOLLOW_
#define _CAMERA_FOLLOW_

#include "Camera.h"

class CameraFollow : public Camera
{
	public:

		CameraFollow(const Position3f* followPosition, const Road* road);

		virtual ~CameraFollow();

		virtual void update(float deltaTimeS) override;

	private:

		const Position3f* followPosition;
};

#endif