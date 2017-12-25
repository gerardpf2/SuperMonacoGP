#ifndef _MODULE_SUPER_MONACO_GP_
#define _MODULE_SUPER_MONACO_GP_

#include "ModuleWorld.h"

class ModuleSuperMonacoGP : public ModuleWorld
{
	public:

		ModuleSuperMonacoGP(GameEngine* gameEngine);

		virtual ~ModuleSuperMonacoGP();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

		virtual void registerLapTime(const Car* car) const override;

	private:

		virtual void addPlayer() override;

		virtual void addCars() override;
};

#endif