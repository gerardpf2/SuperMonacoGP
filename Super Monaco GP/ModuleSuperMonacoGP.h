#ifndef _MODULE_SUPER_MONACO_GP_
#define _MODULE_SUPER_MONACO_GP_

#define CARS_X 7.5f
#define CARS_OFFSET_Z 5.0f
#define PLAYER_POSITION 10
#define WAIT_TIME_GO_RESULTS 2.0f

#include "ModuleWorld.h"

class ModuleSuperMonacoGP : public ModuleWorld
{
	public:

		ModuleSuperMonacoGP(GameEngine* gameEngine);

		virtual ~ModuleSuperMonacoGP();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

		virtual void registerLapTime(const Car* car) override;

	private:

		void registerEstimatedLapTimes() const;

		virtual void addPlayer() override;

		virtual void addCars() override;

		virtual void renderUI() const override;

		void renderLapTimes() const;

		void renderCurrentPosition() const;

	private:

		std::vector<uint> lapsCars;

		std::vector<const Car*> enemyCars;

		bool goResults = false;

		float waitTimeGoResults = 0.0f;

		// UI

		WindowPosition lapTimesPosition;

		WindowPosition positionPosition, positionValuePosition;

		WindowPosition currentLapPosition;
};

#endif