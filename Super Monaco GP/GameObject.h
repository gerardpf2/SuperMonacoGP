#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

#include "Types.h"

enum class GameObjectType
{
	PLAYER,
	CAR,
	ENVIRONMENT,
	ENVIRONMENT_ANIMATED,
};

class Road;
class Camera;
class ModuleRenderer;
class AnimationContainer;

class GameObject
{
	public:

		virtual ~GameObject();

		uint getId() const;

		virtual GameObjectType getType() const = 0;

		const AnimationContainer* getAnimationContainer() const;

		const WorldPosition* getPosition() const;

		void setPosition(const WorldPosition& position);

		const Road* getRoad() const;

		void setRoad(const Road* road);

		void elevate();

		void moveX(float incX);

		virtual void update(float deltaTimeS);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		virtual void cleanUp();

	protected:

		GameObject(uint id, const AnimationContainer* animationContainer);

		void limitZ();

	protected:

		WorldPosition position;

	protected:

		const AnimationContainer* animationContainer = nullptr;

	private:

		uint id;

		Size size;

		const Road* road = nullptr;
};

#endif