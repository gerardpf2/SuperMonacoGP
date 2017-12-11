#ifndef _SEGMENT_
#define _SEGMENT_

#include <set>
#include "Types.h"
// #include "GameObject.h"

/* struct GameObjectPositionZComparer
{
	// Compare using game objects' z position
	bool operator() (const GameObject* gO0, const GameObject* gO1) const
	{
		return gO0->getPosition()->z > gO1->getPosition()->z;
	}
}; */

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

		const std::set<const GameObject* /* , GameObjectPositionZComparer */ >* getGameObjects() const;

		void addGameObject(const GameObject* gameObject);

		void eraseGameObject(const GameObject* gameObject);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer, short& maxWindowY, bool enableClip = true, bool mirror = false) const;

	private:

		uint index;

		float zNear, zFar;

		float curve = 0.0f;

		float yNear = 0.0f, yFar = 0.0f;

		const RumbleColors* rumbleColors = nullptr;

		std::set<const GameObject* /* , GameObjectPositionZComparer */ > gameObjects;

		float xOffsetNear, xOffsetFar;

		float zOffset;

		short clipY;
};

#endif