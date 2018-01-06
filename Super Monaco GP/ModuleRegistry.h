#ifndef _MODULE_REGISTRY_
#define _MODULE_REGISTRY_

#include <vector>
#include "Types.h"
#include "Module.h"

class ModuleRegistry : public Module
{
	public:

		ModuleRegistry(GameEngine* gameEngine);

		virtual ~ModuleRegistry();

		void defaultValues();

		uint getCurrentCourseId() const;

		void setCurrentCourseId(uint courseId);

		float getPlayerBestLapTime() const;

		float getPlayerBestLapTime(uint courseId) const;

		void setPlayerBestLapTime(float time);

		void updatePlayerBestLapTimeIfBetter(float time);

		float getCarLapTime(uint carId, uint lap) const;

		void setCarLapTime(uint carId, uint lap, float time);

		void updateCarLapTimeIfBetter(uint carId, uint lap, float time);

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		uint currentCourseId = 0; // Italy

		/*
		
		For each Course
			Player best Lap time
			For each Car
				For each Lap
					Car Lap time
		
		*/

		std::vector<std::pair<float, std::vector<std::vector<float>>>> values;
};

#endif