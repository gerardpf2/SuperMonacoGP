#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

#include "Types.h"

class Road;
class Camera;
class AnimationGroup;
class ModuleRenderer;

class GameObject
{
	public:

		GameObject(const WorldPosition& position, const AnimationGroup* animationGroup, const Road* road);

		virtual ~GameObject();

		const WorldPosition* getPosition() const;

		const AnimationGroup* getAnimationGroup() const;

		const Road* getRoad() const;

		void elevate();

		virtual void update(float deltaTimeS);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

	protected:

		void limitZ();

	protected:

		WorldPosition position;

	protected:

		const AnimationGroup* animationGroup = nullptr;

	private:

		Size size;

		const Road* road = nullptr;
};

#endif