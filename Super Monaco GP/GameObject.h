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

class Camera;
class ModuleWorld;
class ModuleRenderer;

class GameObject
{
	public:

		virtual ~GameObject();

		virtual GameObjectType getType() const = 0;

		float getGlobalZ() const;

		const WorldPosition* getPosition() const;

		void setPosition(const WorldPosition& position);

		const Box* getBox() const;

		void setBox(const Box& box);

		void defineBox(float mW, float d);

		ModuleWorld* getModuleWorld() const;

		void setModuleWorld(ModuleWorld* moduleWorld);
		
		void elevate();

		void move(const WorldPosition& offset);

		bool getRenderCollisionBox() const;

		void setRenderCollisionBox(bool renderCollisionBox);

		virtual void update(float deltaTimeS);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		virtual void cleanUp();

	protected:

		GameObject();

		void limitZ();

		virtual const Texture* getCurrentTexture(bool mirror) const = 0;

	private:

		void renderCollisionBox(float xOffset, float zOffset, short clipY, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

		void renderCollisionBoxFace(const WorldTrapezoid& worldTrapezoid, short clipY, uint color, const Camera* camera, const ModuleRenderer* moduleRenderer) const;

	protected:

		Box box;

		Size size;

		WorldPosition position;

		float globalZ = 0.0f;

	private:

		bool renderBox = false;

		ModuleWorld* moduleWorld = nullptr;
};

#endif