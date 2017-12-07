#ifndef _BACKGROUND_
#define _BACKGROUND_

#include <SDL_rect.h>

struct Texture;

class Road;
class Camera;
class ModuleRenderer;

class Background
{
	public:

		Background(const Texture* texture, const Texture* textureSky, const Road* road);

		virtual ~Background();

		const Texture* getTexture() const;

		const Texture* getTextureSky() const;

		const Road* getRoad() const;

		float getOffsetX() const;

		void update(const Camera* camera, float deltaTimeS);

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer);

	private:

		const Texture* texture = nullptr;

		const Texture* textureSky = nullptr;

		const Road* road = nullptr;

		SDL_Rect textureRect;

		SDL_Rect renderRect;

		float offsetX = 0.0f;
};

#endif