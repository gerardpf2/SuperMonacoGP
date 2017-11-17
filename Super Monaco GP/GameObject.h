#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

#include "Globals.h"

class Road;
class Camera;
class ModuleRenderer;

class GameObject
{
	public:

		GameObject(const Position3f& position, const Road* road); //

		virtual ~GameObject();

		const Position3f* getPosition() const;

		void elevate(float incY);

		virtual void update(float deltaTimeS);

		void render(float xOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short clipY) const;

	protected:

		// GameObject(const Position3f& position, const Road* road);

		void updateLimitZRoad();

	protected:

		Position3f position;

	private:

		const Road* road = nullptr;
};

#endif