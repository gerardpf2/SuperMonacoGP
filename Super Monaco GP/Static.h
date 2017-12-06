#ifndef _STATIC_
#define _STATIC_

#include "GameObject.h"

struct Texture;

class Static : public GameObject
{
	public:

		Static(uint id, const Texture* texture);

		virtual ~Static();

		virtual GameObjectType getType() const override;

		const Texture* getTexture() const;

		virtual void cleanUp() override;

	private:

		virtual const Texture* getCurrentTexture() const override;

	private:

		const Texture* texture = nullptr;
};

#endif