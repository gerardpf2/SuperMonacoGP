#ifndef _PLAYER_
#define _PLAYER_

#include "Car.h"

class ModuleInput;

class Player : public Car
{
	public:

		Player(const WorldPosition& position, const Road* road, const ModuleInput* moduleInput, Texture* texture);

		virtual ~Player();

	protected:

		virtual void updateDirections(float deltaTimeS) override;

	private:

		const ModuleInput* moduleInput = nullptr;
};

#endif