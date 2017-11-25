#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

#include <vector>
#include "Types.h"

class Road;
class Camera;
class Animation;
class ModuleRenderer;

class GameObject
{
	public:

		// GameObject(const WorldPosition& position, const Texture* texture, const Road* road);
		GameObject(const WorldPosition& position, const std::vector<Animation*>* animations, const Road* road);

		virtual ~GameObject();

		const WorldPosition* getPosition() const;

		// const Texture* getTexture() const;
		const std::vector<Animation*>* getAnimations() const;

		const Road* getRoad() const;

		void elevate();

		virtual void update(float deltaTimeS);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer) const;

	protected:

		void limitZ();

	protected:

		WorldPosition position;

	private:

		Size size;

		const Road* road = nullptr;

		// const Texture* texture = nullptr;

		uint currentAnimation = 0;

		const std::vector<Animation*>* animations = nullptr;
};

#endif