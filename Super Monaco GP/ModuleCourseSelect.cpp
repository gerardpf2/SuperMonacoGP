#include "ModuleCourseSelect.h"

#include "Utils.h"
#include "Animation.h"
#include "ModuleFont.h"
#include "GameEngine.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSwitch.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"
#include "ModuleAnimation.h"

using namespace std;

ModuleCourseSelect::ModuleCourseSelect(GameEngine* gameEngine) :
	Module(gameEngine)
{
	baseRect = BASE_RECT;

	courseRect = COURSE_RECT;

	backgroundRect = BACKGROUND_RECT;

	courseNamePosition = COURSE_NAME_POSITION;

	courseRoundPosition = COURSE_ROUND_POSITION;

	courseRoundValuePosition = COURSE_ROUND_VALUE_POSITION;

	courseLengthPosition = COURSE_LENGTH_POSITION;

	courseLengthValuePosition = COURSE_LENGTH_VALUE_POSITION;

	courseBestLapTimePosition = COURSE_BEST_LAP_TIME_POSITION;

	courseBestLapTimeValuePosition = COURSE_BEST_LAP_TIME_VALUE_POSITION;

	// Change animation rects

	changeCourseAnimationRects.reserve(COURSE_CHANGE_ANIMATION_N_R * COURSE_CHANGE_ANIMATION_N_C + BACKGROUND_CHANGE_ANIMATION_N_R * BACKGROUND_CHANGE_ANIMATION_N_C);

	addChangeAnimationRects(courseRect, COURSE_CHANGE_ANIMATION_N_R, COURSE_CHANGE_ANIMATION_N_C);
	addChangeAnimationRects(backgroundRect, BACKGROUND_CHANGE_ANIMATION_N_R, BACKGROUND_CHANGE_ANIMATION_N_C);
}

ModuleCourseSelect::~ModuleCourseSelect()
{ }

bool ModuleCourseSelect::setUp()
{
	courseSelectAnimationGroupId = getGameEngine()->getModuleAnimation()->load("Resources/Configurations/Animations/CourseSelect.json");

	changeCourseAnimation = getGameEngine()->getModuleAnimation()->getAnimation(courseSelectAnimationGroupId, 0);
	changeCourseAnimation->setTimePercent(0.5f);

	coursesBackgroundsTextureGroupId = getGameEngine()->getModuleTexture()->load("Resources/Configurations/Textures/CoursesBackgrounds.json");

	base = getGameEngine()->getModuleTexture()->get(courseSelectAnimationGroupId, 0);

	courses.reserve(N_COURSES);
	backgrounds.reserve(N_COURSES);

	for(uint i = 0; i < N_COURSES; ++i)
	{
		courses.push_back(getGameEngine()->getModuleTexture()->get(coursesBackgroundsTextureGroupId, 4 * (i + 1) - 1));
		backgrounds.push_back(getGameEngine()->getModuleTexture()->get(coursesBackgroundsTextureGroupId, 4 * i));
	}

	time(getGameEngine()->getModuleRegistry()->getPlayerBestLapTime(currentCourseId), currentBestLapTimeStr);

	audioGroupId = getGameEngine()->getModuleAudio()->load("Resources/Configurations/Audios/CourseSelect.json");

	getGameEngine()->getModuleAudio()->playMusic(audioGroupId, 0);

	return true;
}

bool ModuleCourseSelect::update(float deltaTimeS)
{
	if(!getBlocked())
	{
		checkGoMenu();

		checkSelectOption();

		checkChangeCourse();

		updateBackground(deltaTimeS);

		updateCourseChangeAnimation(deltaTimeS);
	}

	render();

	return true;
}

void ModuleCourseSelect::cleanUp()
{
	getGameEngine()->getModuleAudio()->unload(audioGroupId);

	getGameEngine()->getModuleAnimation()->unload(courseSelectAnimationGroupId);

	getGameEngine()->getModuleTexture()->unload(coursesBackgroundsTextureGroupId);

	base = nullptr;

	courses.clear();

	backgrounds.clear();

	changeCourseAnimation = nullptr;

	changeCourseAnimationRects.clear();
}

void ModuleCourseSelect::addChangeAnimationRects(const SDL_Rect& rect, uint r, uint c)
{
	SDL_Rect tmpRect;
	tmpRect.y = rect.y;

	float totalH = (float)rect.h;

	for(uint rr = 0; rr < r; ++rr)
	{
		float totalW = (float)rect.w;

		tmpRect.x = rect.x;

		tmpRect.h = (int)(totalH / (r - rr));

		for(uint cc = 0; cc < c; ++cc)
		{
			tmpRect.w = (int)(totalW / (c - cc));

			changeCourseAnimationRects.push_back(tmpRect);

			totalW -= tmpRect.w;
			tmpRect.x += tmpRect.w;
		}

		totalH -= tmpRect.h;
		tmpRect.y += tmpRect.h;
	}
}

void ModuleCourseSelect::updateBackground(float deltaTimeS)
{
	backgroundOffsetX = mod0L(backgroundOffsetX + 40.0f * deltaTimeS, (float)backgrounds[currentCourseId]->r->w);
}

void ModuleCourseSelect::updateCourseChangeAnimation(float deltaTimeS)
{
	if(showChangeCourseAnimation)
	{
		if(changeCourseAnimation->hasEnded()) showChangeCourseAnimation = false;
		else changeCourseAnimation->update(deltaTimeS);
	}
}

void ModuleCourseSelect::checkGoMenu() const
{
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
		// getGameEngine()->setGameModule(GameModule::START);
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::START);
}

void ModuleCourseSelect::checkSelectOption() const
{
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		getGameEngine()->getModuleRegistry()->setCurrentCourseId(currentCourseId);
		// getGameEngine()->setGameModule(GameModule::FREE_PRACTICE);
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::FREE_PRACTICE);
	}
}

void ModuleCourseSelect::checkChangeCourse()
{
	int tmpCurrentCourseId = currentCourseId;

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RIGHT) == KeyState::DOWN)
		currentCourseId = mod0L(currentCourseId + 1, N_COURSES);
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_LEFT) == KeyState::DOWN)
		currentCourseId = mod0L(currentCourseId - 1, N_COURSES);

	if(currentCourseId != tmpCurrentCourseId)
	{
		backgroundOffsetX = 0.0f;
		changeCourseAnimation->reset();
		showChangeCourseAnimation = true;
		getGameEngine()->getModuleAudio()->playFx(audioGroupId, 0);
		time(getGameEngine()->getModuleRegistry()->getPlayerBestLapTime(currentCourseId), currentBestLapTimeStr);
	}
}

void ModuleCourseSelect::render() const
{
	renderBase();

	renderCourseInfo();

	renderCourseBackground();

	renderChangeCourseAnimations();
}

void ModuleCourseSelect::renderBase() const
{
	getGameEngine()->getModuleRenderer()->renderTexture(base->t, base->r, &baseRect, base->hFlipped);
}

void ModuleCourseSelect::renderCourseInfo() const
{
	// Name

	getGameEngine()->getModuleFont()->renderText(COURSE_NAMES[currentCourseId], courseNamePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_NAME_POSITION_SCALE, COURSE_NAME_POSITION_SCALE, 248, 216, 0);

	// Round

	getGameEngine()->getModuleFont()->renderText("< ROUND >", courseRoundPosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 252, 252);
	getGameEngine()->getModuleFont()->renderText(to_string(currentCourseId + 1), courseRoundValuePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 216, 0);

	// Length

	getGameEngine()->getModuleFont()->renderText("LENGTH", courseLengthPosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 252, 252);
	
	string courseLengthStr = to_string((int)COURSE_LENGHTS[currentCourseId]); courseLengthStr += "M";
	getGameEngine()->getModuleFont()->renderText(courseLengthStr, courseLengthValuePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 216, 0);

	// Best lap time

	getGameEngine()->getModuleFont()->renderText("BEST LAP", courseBestLapTimePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 252, 252);
	getGameEngine()->getModuleFont()->renderText(currentBestLapTimeStr, courseBestLapTimeValuePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 216, 0);
}

void ModuleCourseSelect::renderCourseBackground() const
{
	// Course

	const Texture* course = courses[currentCourseId];

	getGameEngine()->getModuleRenderer()->renderTexture(course->t, course->r, &courseRect, course->hFlipped);

	// Background

	const Texture* background = backgrounds[currentCourseId];
	SDL_Rect textureRect{ background->r->x, background->r->y, COURSE_BACKGROUND_REGION_X, COURSE_BACKGROUND_REGION_Y };

	SDL_Rect rect0, rect1, renderRect0, renderRect1;

	if(!getRectsEndlessTexture(background, textureRect, backgroundRect, false, backgroundOffsetX, rect0, renderRect0, rect1, renderRect1))
		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect0, &renderRect0, background->hFlipped);
	else
	{
		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect0, &renderRect0, background->hFlipped);
		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect1, &renderRect1, background->hFlipped);
	}

	/*

	const Texture* background = backgrounds[currentCourseId];

	SDL_Rect textureRect{ background->r->x, background->r->y, 208, 72 };

	SDL_Rect rect0 = textureRect;
	rect0.x += (int)backgroundOffsetX;

	if(rect0.x + rect0.w <= background->r->x + background->r->w)
		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect0, &backgroundRect, background->hFlipped);
	else
	{
		rect0.w -= rect0.x + rect0.w - (background->r->x + background->r->w);

		SDL_Rect rect1 = textureRect;
		rect1.w = textureRect.w - rect0.w;

		SDL_Rect renderRect0 = backgroundRect;
		renderRect0.w = (int)(renderRect0.w * ((float)rect0.w / textureRect.w));

		SDL_Rect renderRect1 = backgroundRect;
		renderRect1.x += renderRect0.w;
		renderRect1.w = backgroundRect.w - renderRect0.w;

		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect0, &renderRect0, background->hFlipped);
		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect1, &renderRect1, background->hFlipped);
	}

	*/
}

void ModuleCourseSelect::renderChangeCourseAnimations() const
{
	if(showChangeCourseAnimation)
	{
		for(SDL_Rect rect : changeCourseAnimationRects)
		{
			const Texture* animationT = changeCourseAnimation->getCurrentFrame();
			getGameEngine()->getModuleRenderer()->renderTexture(animationT->t, animationT->r, &rect, animationT->hFlipped);
		}
	}
}