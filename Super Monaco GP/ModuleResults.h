#ifndef _MODULE_RESULTS_
#define _MODULE_RESULTS_

#include <vector>
#include "Types.h"
#include "Module.h"
#include <SDL_rect.h>

class Animation;

struct CarResult;

class ModuleResults : public Module
{
	public:

		ModuleResults(GameEngine* gameEngine);

		virtual ~ModuleResults();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		void addResults();

		void addResultsValuesPositions();

		void addResultsValuesPositions(short x0, short x1, uint n);

		void updateBase(float deltaTimeS);

		void updateCar(float deltaTimeS);

		void checkGoNextCourseOrMenu() const;

		void render() const;

		void renderBase() const;

		void renderCar() const;

		void renderResults() const;

	private:

		uint audioGroupId = -1;

		uint gridTextureGroupId = -1;

		uint baseAnimationGroupId = -1;

		const Texture* grid = nullptr;

		const Texture* car = nullptr;

		Animation* baseAnimation = nullptr;

		std::vector<std::pair<bool, float>> results;

		std::vector<std::pair<bool, std::string>> resultsStr;

		CarResult* carResult = nullptr;

		bool carInUse = true;

		float waitNextCarCounter = 0.0f;

		// UI

		SDL_Rect baseAllRect, baseRect, grid0Rect, grid1Rect;

		WindowPosition resultsPosition;

		WindowPosition rank0TextPosition, rank1TextPosition;

		WindowPosition time0TextPosition, time1TextPosition;

		std::vector<std::pair<WindowPosition, WindowPosition>> resultsValuesPositions;
};

#endif