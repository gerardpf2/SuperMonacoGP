#include "ModuleRegistry.h"

#include <assert.h>
#include "Globals.h"

ModuleRegistry::ModuleRegistry(GameEngine* gameEngine) :
	Module(gameEngine)
{ }

ModuleRegistry::~ModuleRegistry()
{ }

void ModuleRegistry::defaultValues()
{
	if(values.empty())
	{
		values.resize(N_COURSES);

		for(uint i = 0; i < (uint)values.size(); ++i)
		{
			values[i].first = INFINITY;
			values[i].second.resize(N_CARS);

			for(uint j = 0; j < (uint)values[i].second.size(); ++j)
				values[i].second[j].resize(N_LAPS, INFINITY);
		}
	}
	else
	{
		for(uint i = 0; i < (uint)values.size(); ++i)
			for(uint j = 0; j < (uint)values[i].second.size(); ++j)
				for(uint k = 0; k < (uint)values[i].second[j].size(); ++k)
					values[i].second[j][k] = INFINITY;
	}
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
	return getPlayerBestLapTime(currentCourseId);
}

float ModuleRegistry::getPlayerBestLapTime(uint courseId) const
{
	assert((uint)values.size() > courseId);

	return values[courseId].first;
}

void ModuleRegistry::setPlayerBestLapTime(float time)
{
	assert((uint)values.size() > currentCourseId);

	values[currentCourseId].first = time;
}

void ModuleRegistry::updatePlayerBestLapTimeIfBetter(float time)
{
	if(time < getPlayerBestLapTime())
		setPlayerBestLapTime(time);
}

float ModuleRegistry::getCarLapTime(uint carId, uint lap) const
{
	assert((uint)values.size() > currentCourseId);
	assert((uint)values[currentCourseId].second.size() > carId);
	assert((uint)values[currentCourseId].second[carId].size() > lap);

	return values[currentCourseId].second[carId][lap];
}

void ModuleRegistry::setCarLapTime(uint carId, uint lap, float time)
{
	assert((uint)values.size() > currentCourseId);
	assert((uint)values[currentCourseId].second.size() > carId);
	assert((uint)values[currentCourseId].second[carId].size() > lap);

	values[currentCourseId].second[carId][lap] = time;
}

void ModuleRegistry::updateCarLapTimeIfBetter(uint carId, uint lap, float time)
{
	if(time < getCarLapTime(carId, lap))
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