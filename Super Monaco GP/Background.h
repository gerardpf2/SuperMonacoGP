#ifndef _BACKGROUND_
#define _BACKGROUND_

struct Texture;

class Camera;
class ModuleRenderer;

class Background
{
	public:

		Background(const Texture* texture, const Texture* textureSky);

		virtual ~Background();

		const Texture* getTexture() const;

		const Texture* getTextureSky() const;

		float getOffsetX() const;

		void render(const Camera* camera, const ModuleRenderer* moduleRenderer);

	private:

		const Texture* texture = nullptr;

		const Texture* textureSky = nullptr;

		float offsetX = 0.0f;
};

#endif