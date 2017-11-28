#ifndef _PLAYER_
#define _PLAYER_

#include "Car.h"

class ModuleInput;

class Player : public Car
{
	public:

		Player(const WorldPosition& position, const AnimationContainer* animationContainer, const Road* road, const ModuleInput* moduleInput);

		virtual ~Player();

		const ModuleInput* getModuleInput() const;

	protected:

		virtual void updateDirection(float deltaTimeS) override;

	private:

		const ModuleInput* moduleInput = nullptr;
};

#endif