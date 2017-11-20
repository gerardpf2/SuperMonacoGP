#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

#include "Types.h"
#include <SDL_rect.h>

class Road;
class Camera;
class ModuleRenderer;

// struct SDL_Texture;

class GameObject
{
	public:

		GameObject(const WorldPosition& position, const Road* road, TextureInfo& textureInfo); //
		// GameObject(const WorldPosition& position, const Road* road, SDL_Texture* texture, SDL_Rect& textureRect); //

		virtual ~GameObject();

		const WorldPosition* getPosition() const;

		void elevate(float incY);

		virtual void update(float deltaTimeS);

		void render(float xOffset, float zOffset, const Camera* camera, const ModuleRenderer* moduleRenderer, short clipY) const;

	protected:

		// GameObject(const WorldPosition& position, const Road* road);

		void updateLimitZRoad();

	protected:

		WorldPosition position;

	private:

		const Road* road = nullptr;

		TextureInfo textureInfo;

		// SDL_Texture* texture;

		// SDL_Rect textureRect;

		Size size;
};

#endif