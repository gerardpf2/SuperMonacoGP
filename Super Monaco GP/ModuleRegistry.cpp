#include "ModuleRegistry.h"

ModuleRegistry::ModuleRegistry(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleRegistry::~ModuleRegistry()
{ }

void ModuleRegistry::defaultValues()
{
	values.resize(N_COURSES);

	for(uint i = 0; i < (uint)values.size(); ++i)
	{
		values[i].second.resize(N_CARS);

		for(uint j = 0; j < (uint)values[i].second.size(); ++j)
			values[i].second[j].resize(N_LAPS, -1.0f);
	}

	values[0].first = PLAYER_BEST_LAP_TIME_COURSE_0;
	values[1].first = PLAYER_BEST_LAP_TIME_COURSE_1;
}

uint ModuleRegistry::getCurrentCourseId() const
{
	return currentCourseId;
}

void ModuleRegistry::setCurrentCourseId(uint courseId)
{
	currentCourseId = courseId;
}

float ModuleRegistry::getPlayerBestLapTime() const
{
	return values[currentCourseId].first;
}

void ModuleRegistry::setPlayerBestLapTime(float time)
{
	values[currentCourseId].first = time;
}

void ModuleRegistry::updatePlayerBestLapTimeIfBetter(float time)
{
	if(time < getPlayerBestLapTime())
		setPlayerBestLapTime(time);
}

float ModuleRegistry::getCarLapTime(uint carId, uint lap) const
{
	return values[currentCourseId].second[carId][lap];
}

void ModuleRegistry::setCarLapTime(uint carId, uint lap, float time)
{
	values[currentCourseId].second[carId][lap] = time;
}

void ModuleRegistry::updateCarLapTimeIfBetter(uint carId, uint lap, float time)
{
	float previousTime = getCarLapTime(carId, lap);

	if(previousTime == -1.0f || time < previousTime)
		setCarLapTime(carId, lap, time);
}

bool ModuleRegistry::setUp()
{
	defaultValues();

	return true;
}

void ModuleRegistry::cleanUp()
{
	values.clear();
}