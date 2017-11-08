#ifndef _GAME_OBJECT_
#define _GAME_OBJECT_

class GameObject
{
	public:

		GameObject(float z, unsigned int color);

		virtual ~GameObject();

		float getZ() const;

	private:

		float z;

		unsigned int color;
};

#endif