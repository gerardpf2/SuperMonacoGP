#ifndef _PLAYER_
#define _PLAYER_

#include "Car.h"

class ModuleInput;

class Player : public Car
{
	public:

		Player(uint id, const AnimationContainer* animationContainer, const ModuleInput* moduleInput);

		virtual ~Player();

		virtual GameObjectType getType() const override;

		const ModuleInput* getModuleInput() const;

	protected:

		virtual void updateDirection(float deltaTimeS) override;

	private:

		const ModuleInput* moduleInput = nullptr;
};

#endif