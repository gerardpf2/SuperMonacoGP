#ifndef _MODULE_REGISTRY_
#define _MODULE_REGISTRY_

#include <vector>
#include "Types.h"
#include "Module.h"

#define N_COURSES 2
#define N_CARS 16
#define N_LAPS 3
#define PLAYER_BEST_LAP_TIME_COURSE_0 300.0f // Ajustar
#define PLAYER_BEST_LAP_TIME_COURSE_1 400.0f // Ajustar

class ModuleRegistry : public Module
{
	public:

		ModuleRegistry(GameEngine* gameEngine);

		virtual ~ModuleRegistry();

		void defaultValues();

		uint getCurrentCourseId() const;

		void setCurrentCourseId(uint courseId);

		float getPlayerBestLapTime() const;

		void setPlayerBestLapTime(float time);

		void updatePlayerBestLapTimeIfBetter(float time);

		float getCarLapTime(uint carId, uint lap) const;

		void setCarLapTime(uint carId, uint lap, float time);

		void updateCarLapTimeIfBetter(uint carId, uint lap, float time);

		virtual bool setUp() override;

		virtual void cleanUp() override;

	private:

		uint currentCourseId = 0;

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