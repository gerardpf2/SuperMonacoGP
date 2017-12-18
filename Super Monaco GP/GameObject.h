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
class ModuleWorld;
class ModuleRenderer;

class GameObject
{
	public:

		virtual ~GameObject();

		uint getId() const;

		virtual GameObjectType getType() const = 0;

		const WorldPosition* getPosition() const;

		void setPosition(const WorldPosition& position);

		const Collider* getCollider() const;

		// const Road* getRoad() const;

		// void setRoad(const Road* road);

		const ModuleWorld* getModuleWorld() const;

		void setModuleWorld(const ModuleWorld* moduleWorld);
		
		void elevate();

		// void moveX(float incX);

		void move(const WorldPosition& offset);

		virtual void update(float deltaTimeS);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		virtual void cleanUp();

	protected:

		GameObject(uint id);

		void limitZ();

		virtual const Texture* getCurrentTexture(bool mirror) const = 0;

	private:

		void _DEBUG_renderCollider_DEBUG_(float xOffset, float zOffset, short clipY, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void _DEBUG_renderColliderFace_DEBUG_(const WorldTrapezoid& worldTrapezoid, short clipY, uint color, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

	protected:

		Size size;

		Collider collider;

		WorldPosition position;

	private:

		uint id;

		// const Road* road = nullptr;

		const ModuleWorld* moduleWorld = nullptr;
};

#endif