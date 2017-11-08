#ifndef _SEGMENT_
#define _SEGMENT_

class Camera;
class ModuleRenderer;

class Segment
{
	public:

		Segment(float zNear, unsigned int color);

		virtual ~Segment();

		float getZNear() const;

		float getZFar() const;

		void render(float offset, const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxScreenY) const;

	private:

		float zNear, zFar;

	public: //

		unsigned int color;
};

#endif