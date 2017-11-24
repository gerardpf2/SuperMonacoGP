#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

#include "Types.h"

class Road;
class Camera;
class ModuleRenderer;

class GameObject
{
	public:

		GameObject(const WorldPosition& position, const Road* road, Texture* texture);

		virtual ~GameObject();

		const WorldPosition* getPosition() const;

		void elevate(float incY);

		virtual void update(float deltaTimeS);

		virtual void render(float xOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short clipY) const;

	protected:

		void limitZ();

	protected:

		WorldPosition position;

		const Road* road = nullptr;

	private:

		Size size;

		Texture* texture = nullptr;
};

#endif