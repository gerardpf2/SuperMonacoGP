#include "Minimap.h"

#include "Car.h"
#include "Road.h"
#include "Utils.h"
#include "MinimapUI.h"
#include "ModuleJson.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"

using namespace std;
using namespace rapidjson;

Minimap::Minimap(const Road* road) :
	road(road)
{
	courseRect = COURSE_RECT;
}

Minimap::~Minimap()
{ }

const Road* Minimap::getRoad() const
{
	return road;
}

void Minimap::registerCar(const Car* car)
{
	registeredCars.insert(car);
}

void Minimap::load(const char* jsonPath, const ModuleJson* moduleJson, ModuleTexture* moduleTexture)
{
	assert(jsonPath);
	assert(moduleJson);
	assert(moduleTexture);

	Document jsonDocument;
	moduleJson->read(jsonPath, jsonDocument);

	const Value& courseJson = jsonDocument["course"];
	const char* textureGroupPath = courseJson["textureGroupPath"].GetString();
	uint textureId = courseJson["textureId"].GetUint();
	uint texturePlayer = courseJson["texturePlayer"].GetUint();
	uint textureEnemyCar = courseJson["textureEnemyCar"].GetUint();
	const Value& pointsJson = jsonDocument["points"];

	textureGroupId = moduleTexture->load(textureGroupPath);

	course = moduleTexture->get(textureGroupId, textureId);
	playerIcon = moduleTexture->get(textureGroupId, texturePlayer);
	enemyCarIcon = moduleTexture->get(textureGroupId, textureEnemyCar);

	addPoints(pointsJson);
}

void Minimap::unload(ModuleTexture* moduleTexture)
{
	assert(moduleTexture);

	road = nullptr;

	points.clear();

	registeredCars.clear();

	moduleTexture->unload(textureGroupId);

	course = nullptr;

	playerIcon = nullptr;

	enemyCarIcon = nullptr;
}

void Minimap::render(const ModuleRenderer* moduleRenderer) const
{
	assert(course);
	assert(moduleRenderer);

	moduleRenderer->renderTexture(course->t, course->r, &courseRect);

	renderCarIcons(moduleRenderer);
}

void Minimap::addPoints(const Value& pointsJson)
{
	if(!pointsJson.Empty())
	{
		// Compute minimapTotalLength based on all the defined points

		float minimapTotalLength = 0.0f;

		int initialX = pointsJson[0]["x"].GetInt();
		int initialY = pointsJson[0]["y"].GetInt();

		int previousX = initialX;
		int previousY = initialY;

		for(SizeType i = 1; i < pointsJson.Size(); ++i)
		{
			const Value& point = pointsJson[i];

			int x = point["x"].GetInt();
			int y = point["y"].GetInt();

			minimapTotalLength += sqrtf(powf((float)(x - previousX), 2.0f) + powf((float)(y - previousY), 2.0f));

			previousX = x;
			previousY = y;
		}

		// Compute all the needed ranges

		float accumulatedDistance = 0.0f;

		previousX = initialX;
		previousY = initialY;

		for(SizeType i = 0; i < pointsJson.Size(); ++i)
		{
			const Value& point = pointsJson[i];

			int x = point["x"].GetInt();
			int y = point["y"].GetInt();

			float distance = sqrtf(powf((float)(x - previousX), 2.0f) + powf((float)(y - previousY), 2.0f));

			accumulatedDistance += distance;
			points[accumulatedDistance / minimapTotalLength] = pair<int, int>(x, y);

			previousX = x;
			previousY = y;
		}
	}
}

void Minimap::findPoint(float lengthPercent, int& x, int& y) const
{
	map<float, pair<int, int>>::const_iterator it = points.upper_bound(lengthPercent);
	
	map<float, pair<int, int>>::const_iterator it0 = it;
	map<float, pair<int, int>>::const_iterator it1 = --it;

	if(it0 == points.end()) it0 = it1;

	x = (int)interpolate(lengthPercent, it1->first, it0->first, (float)it1->second.first, (float)it0->second.first);
	y = (int)interpolate(lengthPercent, it1->first, it0->first, (float)it1->second.second, (float)it0->second.second);

	x = (int)(x * COURSE_RECT_SCALE);
	y = (int)(y * COURSE_RECT_SCALE);

	x += COURSE_RECT_X;
	y += COURSE_RECT_Y;
}

void Minimap::renderCarIcons(const ModuleRenderer* moduleRenderer) const
{
	const Car* player = nullptr;

	for(const Car* car : registeredCars)
	{
		assert(car);

		if(car->getType() == GameObjectType::PLAYER) player = car;
		else renderCarIcon(car, moduleRenderer);
	}

	if(player) renderCarIcon(player, moduleRenderer);
}

void Minimap::renderCarIcon(const Car* car, const ModuleRenderer* moduleRenderer) const
{
	assert(car);
	assert(road);
	assert(moduleRenderer);

	int w, h;
	const Texture* t;

	if(car->getType() == GameObjectType::CAR)
	{
		t = enemyCarIcon;
		w = ENEMY_CAR_ICON_W;
		h = ENEMY_CAR_ICON_H;
	}
	else
	{
		t = playerIcon;
		w = PLAYER_ICON_W;
		h = PLAYER_ICON_H;
	}

	float carPositionZPercent = car->getPosition()->z / road->getLength();

	int x, y; findPoint(carPositionZPercent, x, y);
	SDL_Rect carRect{ (int)(x - w / 2), (int)(y - h / 2), w, h };

	assert(t);

	moduleRenderer->renderTexture(t->t, t->r, &carRect);
}