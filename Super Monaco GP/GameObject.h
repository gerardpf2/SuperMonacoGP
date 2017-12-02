#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

#include "Types.h"

enum class GameObjectType
{
	PLAYER,
	CAR,
	STATIC,
	ANIMATED,
};

class Road;
class Camera;
class ModuleRenderer;

class GameObject
{
	public:

		virtual ~GameObject();

		uint getId() const;

		virtual GameObjectType getType() const = 0;

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

		GameObject(uint id);

		void limitZ();

		virtual const Texture* getCurrentTexture() const = 0;

	protected:

		Size size;

		WorldPosition position;

	private:

		uint id;

		const Road* road = nullptr;
};

#endif