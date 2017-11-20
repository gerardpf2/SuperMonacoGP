#ifndef _SEGMENT_
#define _SEGMENT_

#include <vector>
#include "Types.h"

using namespace std;

class Camera;
class GameObject;
class ModuleRenderer;

class Segment
{
	public:

		Segment(unsigned int index, float zNear /* , unsigned int color */);

		virtual ~Segment();

		unsigned int getIndex() const;

		float getZNear() const;

		float getZFar() const;

		void render(float xOffset, float dXOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxScreenY) const;

	private:

		unsigned int index;

		float zNear, zFar;

	public: //

		// unsigned int color;

		// unsigned int color0, color1, color2, color3, color4;

		RumbleColors* rumbleColors;

		float dX = 0.0f;

		float yNear = 0.0f;

		float yFar = 0.0f;

		short clipY;

		vector<GameObject*> gameObjects;

		float xOffset, zOffset;
};

#endif