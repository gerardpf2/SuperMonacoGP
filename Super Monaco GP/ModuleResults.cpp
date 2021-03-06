#include "ModuleResults.h"

#include "Utils.h"
#include <algorithm>
#include "Animation.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSwitch.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"
#include "ModuleAnimation.h"
#include "ModuleResultsUI.h"

using namespace std;

struct CarResult
{
	// Simple structure that helps managing the cars that appear in this menu

	float x, y;

	CarResult()
	{
		reset();
	}

	void reset()
	{
		x = CAR_START_POSITION_X + 0.1f * CAR_STEP_X;
		y = CAR_START_POSITION_Y + 0.1f * CAR_STEP_Y;
	}

	bool isOut()
	{
		return x >= CAR_END_POSITION_X || y >= CAR_END_POSITION_Y;
	}

	float distancePercent()
	{
		float xx = x - CAR_START_POSITION_X;
		float yy = y - CAR_START_POSITION_Y;

		return sqrtf(xx * xx + yy * yy) / CAR_DISTANCE;
	}

	void update(float deltaTimeS)
	{
		float velocityExtraMultiplier = distancePercent();

		x += velocityExtraMultiplier * CAR_VELOCITY * CAR_STEP_X * deltaTimeS;
		y += velocityExtraMultiplier * CAR_VELOCITY * CAR_STEP_Y * deltaTimeS;
	}
};

ModuleResults::ModuleResults(GameEngine* gameEngine) :
	Module(gameEngine)
{
	baseAllRect = BASE_ALL_RECT;

	baseRect = BASE_RECT;

	grid0Rect = GRID_0_RECT;

	grid1Rect = GRID_1_RECT;

	resultsPosition = RESULTS_POSITION;

	rank0TextPosition = RANK_0_POSITION;
	rank1TextPosition = RANK_1_POSITION;

	time0TextPosition = TIME_0_POSITION;
	time1TextPosition = TIME_1_POSITION;

	addResultsValuesPositions();

	carResult = new CarResult();
}

ModuleResults::~ModuleResults()
{ }

bool ModuleResults::setUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAudio());
	assert(getGameEngine()->getModuleTexture());
	assert(getGameEngine()->getModuleAnimation());

	baseAnimationGroupId = getGameEngine()->getModuleAnimation()->load("Resources/Configurations/Animations/Results.json");

	baseAnimation = getGameEngine()->getModuleAnimation()->getAnimation(baseAnimationGroupId, 0);

	grid = getGameEngine()->getModuleTexture()->get(baseAnimationGroupId, 3);

	car = getGameEngine()->getModuleTexture()->get(baseAnimationGroupId, 4);

	addResults();

	audioGroupId = getGameEngine()->getModuleAudio()->load("Resources/Configurations/Audios/Results.json");

	getGameEngine()->getModuleAudio()->playMusic(audioGroupId, 0);

	return true;
}

bool ModuleResults::update(float deltaTimeS)
{
	if(!getBlocked())
	{
		checkGoNextCourseOrMenu();

		updateBase(deltaTimeS);

		updateCar(deltaTimeS);
	}

	render();

	return true;
}

void ModuleResults::cleanUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAudio());
	assert(getGameEngine()->getModuleAnimation());

	getGameEngine()->getModuleAudio()->unload(audioGroupId);

	getGameEngine()->getModuleAnimation()->unload(baseAnimationGroupId);

	baseAnimation = nullptr;

	grid = nullptr;

	car = nullptr;

	results.clear();

	resultsStr.clear();

	resultsValuesPositions.clear();

	delete carResult; carResult = nullptr;
}

void ModuleResults::addResults()
{
	assert(getGameEngine());

	ModuleRegistry* moduleRegistry = getGameEngine()->getModuleRegistry();

	assert(moduleRegistry);

	results.reserve(N_CARS);

	for(uint i = 0; i < N_CARS; ++i)
	{
		float totalTime = 0.0f;

		for(uint j = 0; j < N_LAPS; ++j)
			totalTime += moduleRegistry->getCarLapTime(i, j);

		results.push_back(pair<bool, float>(i == 0, totalTime)); // Player id is 0
	}

	sort(results.begin(), results.end(), [](const pair<bool, float>& result0, const pair<bool, float>& result1) -> bool { return result0.second < result1.second; });

	resultsStr.reserve(N_CARS);

	for(pair<bool, float> result : results)
	{
		string timeStr; time(result.second, timeStr);
		resultsStr.push_back(pair<bool, string>(result.first, timeStr));
	}
}

void ModuleResults::addResultsValuesPositions()
{
	resultsValuesPositions.reserve(N_CARS);

	addResultsValuesPositions(RESULT_0_0_POSITION_X, RESULT_0_1_POSITION_X, N_CARS / 2);
	addResultsValuesPositions(RESULT_1_0_POSITION_X, RESULT_1_1_POSITION_X, N_CARS / 2);
}

void ModuleResults::addResultsValuesPositions(short x0, short x1, uint n)
{
	short y = RESULT_POSITION_Y;

	for(uint i = 0; i < n; ++i)
	{
		resultsValuesPositions.push_back(pair<WindowPosition, WindowPosition>(WindowPosition{ x0, y }, WindowPosition{ x1, y }));
		
		y += RESULT_POSITION_H;
	}
}

void ModuleResults::updateBase(float deltaTimeS)
{
	assert(baseAnimation);

	baseAnimation->update(deltaTimeS);
}

void ModuleResults::updateCar(float deltaTimeS)
{
	if(carInUse)
	{
		carResult->update(deltaTimeS);

		if(carResult->isOut())
		{
			assert(getGameEngine());
			assert(getGameEngine()->getModuleAudio());

			carResult->reset();

			carInUse = false;
			waitNextCarCounter = 0.0f;

			getGameEngine()->getModuleAudio()->playFx(audioGroupId, 0);
		}
	}
	else if((waitNextCarCounter += deltaTimeS) >= TIME_BETWEEN_CARS) carInUse = true;
}

void ModuleResults::checkGoNextCourseOrMenu() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());
	assert(getGameEngine()->getModuleSwitch());
	assert(getGameEngine()->getModuleRegistry());

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		getGameEngine()->getModuleRegistry()->setCurrentCourseId((getGameEngine()->getModuleRegistry()->getCurrentCourseId() + 1) % N_COURSES);
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::SUPER_MONACO_GP);
	}
	else if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::START);
}

void ModuleResults::render() const
{
	renderBase();

	renderCar();

	renderResults();
}

void ModuleResults::renderBase() const
{
	assert(baseAnimation);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	getGameEngine()->getModuleRenderer()->renderRect(&baseAllRect, 176, 180, 144);

	const Texture* baseT = baseAnimation->getCurrentFrame();

	assert(baseT);

	getGameEngine()->getModuleRenderer()->renderTexture(baseT->t, baseT->r, &baseRect, baseT->hFlipped);
}

void ModuleResults::renderCar() const
{
	if(!carInUse) return;

	assert(car);
	assert(car->r);
	assert(carResult);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	float scale = interpolate(carResult->distancePercent(), 0.0f, 1.0f, CAR_START_SCALE, CAR_END_SCALE);

	int x = (int)carResult->x;
	int y = (int)carResult->y;
	int w = (int)(scale * car->r->w);
	int h = (int)(scale * car->r->h);

	y -= h;

	SDL_Rect carRect{ x, y, w, h };

	getGameEngine()->getModuleRenderer()->renderTexture(car->t, car->r, &carRect, car->hFlipped);
}

void ModuleResults::renderResults() const
{
	assert(grid);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	ModuleFont* moduleFont = getGameEngine()->getModuleFont();

	assert(moduleFont);

	moduleFont->renderText("RESULTS", resultsPosition, HAlignment::CENTER, VAlignment::BOTTOM, RESULTS_POSITION_SCALE, RESULTS_POSITION_SCALE, 248, 36, 32);

	getGameEngine()->getModuleRenderer()->renderTexture(grid->t, grid->r, &grid0Rect, grid->hFlipped);
	getGameEngine()->getModuleRenderer()->renderTexture(grid->t, grid->r, &grid1Rect, grid->hFlipped);

	moduleFont->renderText("RANK", rank0TextPosition, HAlignment::CENTER, VAlignment::BOTTOM, RANK_TIME_POSITION_SCALE_W, RANK_TIME_POSITION_SCALE_H, 32, 252, 104);
	moduleFont->renderText("RANK", rank1TextPosition, HAlignment::CENTER, VAlignment::BOTTOM, RANK_TIME_POSITION_SCALE_W, RANK_TIME_POSITION_SCALE_H, 32, 252, 104);
	moduleFont->renderText("T. TIME", time0TextPosition, HAlignment::CENTER, VAlignment::BOTTOM, RANK_TIME_POSITION_SCALE_W, RANK_TIME_POSITION_SCALE_H, 32, 252, 104);
	moduleFont->renderText("T. TIME", time1TextPosition, HAlignment::CENTER, VAlignment::BOTTOM, RANK_TIME_POSITION_SCALE_W, RANK_TIME_POSITION_SCALE_H, 32, 252, 104);

	for(uint i = 0; i < (uint)resultsValuesPositions.size(); ++i)
	{
		assert((uint)resultsStr.size() > i);

		bool isPlayer = resultsStr[i].first;
		string resultValue = resultsStr[i].second;
		string resultIndex; rankStr(i + 1, resultIndex);

		Uint8 r = 248;
		Uint8 g = isPlayer ? 36 : 216;
		Uint8 b = isPlayer ? 32 : 0;

		moduleFont->renderText(resultIndex, resultsValuesPositions[i].first, HAlignment::CENTER, VAlignment::BOTTOM, RESULT_POSITION_SCALE_W, RESULT_POSITION_SCALE_H, r, g, b);
		moduleFont->renderText(resultValue, resultsValuesPositions[i].second, HAlignment::CENTER, VAlignment::BOTTOM, RESULT_POSITION_SCALE_W, RESULT_POSITION_SCALE_H, r, g, b);
	}
}