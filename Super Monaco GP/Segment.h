#ifndef _SEGMENT_
#define _SEGMENT_

#include <vector>

using namespace std;

class Camera;
class GameObject;
class ModuleRenderer;

class Segment
{
	public:

		Segment(unsigned int index, float zNear, unsigned int color);

		virtual ~Segment();

		unsigned int getIndex() const;

		float getZNear() const;

		float getZFar() const;

		void render(float xOffset, float dXOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxScreenY) const;

	private:

		unsigned int index;

		float zNear, zFar;

	public: //

		unsigned int color;

		float dX = 0.0f;

		float yNear = 0.0f;

		float yFar = 0.0f;

		short clipY;

		vector<GameObject*> gameObjects;
};

#endif