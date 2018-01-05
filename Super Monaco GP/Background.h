#ifndef _BACKGROUND_
#define _BACKGROUND_

#include <SDL_rect.h>

struct Texture;

class Road;
class Player;
class ModuleRenderer;

class Background
{
	public:

		Background(const Texture* texture, const Texture* textureSky, const Texture* textureGround, float projectionY1);

		virtual ~Background();

		const Texture* getTexture() const;

		const Texture* getTextureSky() const;

		const Texture* getTextureGround() const;

		float getOffsetX() const;

		float getOffsetY() const;

		void update(const Player* player, const Road* road, float deltaTimeS);

		void render(bool mirror, const ModuleRenderer* moduleRenderer) const;

	private:

		const Texture* texture = nullptr; // Constructor

		const Texture* textureSky = nullptr; // Constructor

		const Texture* textureGround = nullptr; // Constructor

		SDL_Rect textureRect;

		SDL_Rect renderTextureRect;
		
		SDL_Rect renderTextureSkyRect;

		SDL_Rect renderTextureGroundRect;

		float offsetX = 0.0f;

		float offsetY = 0.0f;
};

#endif