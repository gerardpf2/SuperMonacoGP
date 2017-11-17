#ifndef _PLAYER_
#define _PLAYER_

#include "GameObject.h"

class ModuleInput;

class Player : public GameObject
{
	public:

		Player(const Position3f& position, const Road* road, const ModuleInput* moduleInput);

		virtual ~Player();

		virtual void update(float deltaTimeS) override;

	private:

		void moveX(float incX);

		void moveZ(float incZ);

	private:

		const ModuleInput* moduleInput = nullptr;
};

#endif