#ifndef _MINIMAP_
#define _MINIMAP_

#include <set>
#include <map>
#include "Types.h"
#include "Globals.h"
#include <SDL_rect.h>
#include "rapidjson/document.h"

#define COURSE_RECT_SCALE (1.25f * WINDOW_SCALE)
#define COURSE_RECT_X (int)(490 * WINDOW_SCALE)
#define COURSE_RECT_Y (int)(140 * WINDOW_SCALE)
#define COURSE_RECT_W (int)(112 * COURSE_RECT_SCALE)
#define COURSE_RECT_H (int)(176 * COURSE_RECT_SCALE)
#define COURSE_RECT SDL_Rect{ COURSE_RECT_X, COURSE_RECT_Y, COURSE_RECT_W, COURSE_RECT_H }

#define PLAYER_ICON_W (int)(0.8f * 12 * WINDOW_SCALE)
#define PLAYER_ICON_H (int)(0.8f * 14 * WINDOW_SCALE)

#define ENEMY_CAR_ICON_W (int)(0.8f * 10 * WINDOW_SCALE)
#define ENEMY_CAR_ICON_H (int)(0.8f * 12 * WINDOW_SCALE)

class Car;
class Road;
class ModuleJson;
class ModuleTexture;
class ModuleRenderer;

class Minimap
{
	public:

		Minimap(const Road* road);

		virtual ~Minimap();

		const Road* getRoad() const;

		void registerCar(const Car* car);

		void load(const char* jsonPath, const ModuleJson* moduleJson, ModuleTexture* moduleTexture);

		void unload(ModuleTexture* moduleTexture);

		void render(const ModuleRenderer* moduleRenderer) const;

	private:

		void addPoints(const rapidjson::Value& pointsJson);

		void findPoint(float lengthPercent, int& x, int& y) const;

		void renderCarIcons(const ModuleRenderer* moduleRenderer) const;

		void renderCarIcon(const Car* car, const ModuleRenderer* moduleRenderer) const;

	private:

		uint textureGroupId = -1;

		const Road* road = nullptr;

		std::set<const Car*> registeredCars;

		std::map<float, std::pair<int, int>> points;

		const Texture* course = nullptr;

		const Texture* playerIcon = nullptr;

		const Texture* enemyCarIcon = nullptr;

		SDL_Rect courseRect;
};

#endif