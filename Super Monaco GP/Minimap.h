#ifndef _MINIMAP_
#define _MINIMAP_

#include <set>
#include <map>
#include "Types.h"
#include <SDL_rect.h>
#include "rapidjson/document.h"

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

		const Road* road = nullptr; // Constructor

		uint textureGroupId = -1;

		std::set<const Car*> registeredCars;

		std::map<float, std::pair<int, int>> points;

		const Texture* course = nullptr;

		const Texture* playerIcon = nullptr;

		const Texture* enemyCarIcon = nullptr;

		SDL_Rect courseRect;
};

#endif