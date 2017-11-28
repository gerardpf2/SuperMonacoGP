#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

#include "Types.h"

class Road;
class Camera;
class ModuleRenderer;
class AnimationContainer;

class GameObject
{
	public:

		GameObject(const WorldPosition& position, const AnimationContainer* animationContainer, const Road* road);

		virtual ~GameObject();

		const WorldPosition* getPosition() const;

		const AnimationContainer* getAnimationContainer() const;

		const Road* getRoad() const;

		void elevate();

		virtual void update(float deltaTimeS);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

	protected:

		void limitZ();

	protected:

		WorldPosition position;

	protected:

		const AnimationContainer* animationContainer = nullptr;

	private:

		Size size;

		const Road* road = nullptr;
};

#endif