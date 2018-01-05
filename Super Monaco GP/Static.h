#ifndef _STATIC_
#define _STATIC_

#include "GameObject.h"

struct Texture;

class Static : public GameObject
{
	public:

		Static(const Texture* texture);

		virtual ~Static();

		virtual GameObjectType getType() const override;

		const Texture* getTexture() const;

		virtual void cleanUp() override;

	private:

		virtual const Texture* getCurrentTexture(bool mirror) const override;

	private:

		const Texture* texture = nullptr; // Constructor
};

#endif