#ifndef _CAMERA_FREE_
#define _CAMERA_FREE_

#include "Camera.h"

class ModuleInput;

class CameraFree : public Camera
{
	public:

		CameraFree(const Road* road, const ModuleInput* moduleInput);

		virtual ~CameraFree();

		virtual void update(float deltaTimeS) override;

	private:

		void moveY(float incY);

		void moveZ(float incZ);

	private:

		const ModuleInput* moduleInput;
};

#endif