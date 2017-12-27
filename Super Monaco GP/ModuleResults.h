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

		void checkGoMenuOrNextCourse() const;

		void render() const;

		void renderBase() const;

		void renderCar() const;

		void renderResults() const;

	private:

		uint gridTextureGroupId = -1;

		uint baseAnimationGroupId = -1;

		const Texture* grid = nullptr;

		const Texture* car = nullptr;

		Animation* baseAnimation = nullptr;

		SDL_Rect baseAllRect, baseRect, grid0Rect, grid1Rect;

		WindowPosition resultsPosition;

		std::vector<std::pair<bool, float>> results;

		std::vector<std::pair<bool, std::string>> resultsStr;

		WindowPosition rank0TextPosition, rank1TextPosition;

		WindowPosition time0TextPosition, time1TextPosition;

		std::vector<std::pair<WindowPosition, WindowPosition>> resultsValuesPositions;

		CarResult* carResult = nullptr;

		bool carInUse = true;

		float waitNextCarCounter = 0.0f;
};

#endif