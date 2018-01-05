#ifndef _SEGMENT_
#define _SEGMENT_

#include <set>
#include "Types.h"

class Camera;
class GameObject;
class ModuleRenderer;

class Segment
{
	public:

		Segment(uint index);

		virtual ~Segment();

		uint getIndex() const;

		float getZNear() const;

		float getZFar() const;

		float getCurve() const;

		void setCurve(float curve);

		float getYNear() const;

		void setYNear(float yNear);

		float getYFar() const;

		void setYFar(float yFar);

		float getXOffsetNear() const;

		void setXOffsetNear(float xOffsetNear);

		float getXOffsetFar() const;

		void setXOffsetFar(float xOffsetFar);

		float getZOffset() const;

		void setZOffset(float zOffset);

		short getClipY() const;

		void setClipY(short clipY);

		const RumbleColors* getRumbleColors() const;

		void setRumbleColors(const RumbleColors* rumbleColors);

		const std::set<const GameObject*>* getGameObjects() const;

		void addGameObject(const GameObject* gameObject);

		void eraseGameObject(const GameObject* gameObject);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxWindowY, bool enableClip = true, bool mirror = false) const;

	private:

		uint index; // Constructor

		float zNear = 0.0f, zFar = 0.0f;

		float curve = 0.0f;

		float yNear = 0.0f, yFar = 0.0f;

		const RumbleColors* rumbleColors = nullptr;

		std::set<const GameObject*> gameObjects;

		float xOffsetNear = 0.0f, xOffsetFar = 0.0f;

		float zOffset = 0.0f;

		short clipY = 0;
};

#endif