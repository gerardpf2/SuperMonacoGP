#ifndef _PLAYER_
#define _PLAYER_

#include "Car.h"

class ModuleInput;
class AnimationGrid;

class Player : public Car
{
	public:

		Player(/* uint id, */ AnimationContainer* animationContainer, const ModuleInput* moduleInput);

		virtual ~Player();

		virtual GameObjectType getType() const override;

		const ModuleInput* getModuleInput() const;

		virtual void cleanUp() override;

	protected:

		virtual void updateDirection(float deltaTimeS) override;

		virtual void updateCurrentAnimation(float deltaTimeS) const override;
		
		virtual void updateOffsetX(float dX, float velocityPercent, float curve) override;

	private:

		virtual const Texture* getCurrentTexture(bool mirror) const override;

	private:

		const ModuleInput* moduleInput = nullptr;

		AnimationGrid* animationGrid = nullptr;
};

#endif