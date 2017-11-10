#ifndef _SEGMENT_
#define _SEGMENT_

class Camera;
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
};

#endif