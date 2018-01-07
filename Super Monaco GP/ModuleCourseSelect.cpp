#include "ModuleCourseSelect.h"

#include "Utils.h"
#include "Animation.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSwitch.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"
#include "ModuleAnimation.h"
#include "ModuleCourseSelectUI.h"

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

	changeCourseAnimationRects.reserve(COURSE_CHANGE_ANIMATION_N_R * COURSE_CHANGE_ANIMATION_N_C + BACKGROUND_CHANGE_ANIMATION_N_R * BACKGROUND_CHANGE_ANIMATION_N_C);

	addChangeAnimationRects(courseRect, COURSE_CHANGE_ANIMATION_N_R, COURSE_CHANGE_ANIMATION_N_C);
	addChangeAnimationRects(backgroundRect, BACKGROUND_CHANGE_ANIMATION_N_R, BACKGROUND_CHANGE_ANIMATION_N_C);
}

ModuleCourseSelect::~ModuleCourseSelect()
{ }

bool ModuleCourseSelect::setUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAudio());
	assert(getGameEngine()->getModuleRegistry());
	assert(getGameEngine()->getModuleAnimation());

	courseSelectAnimationGroupId = getGameEngine()->getModuleAnimation()->load("Resources/Configurations/Animations/CourseSelect.json");

	changeCourseAnimation = getGameEngine()->getModuleAnimation()->getAnimation(courseSelectAnimationGroupId, 0);
	changeCourseAnimation->setTimePercent(0.5f);

	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	assert(moduleTexture);

	coursesBackgroundsTextureGroupId = moduleTexture->load("Resources/Configurations/Textures/CoursesBackgrounds.json");

	base = moduleTexture->get(courseSelectAnimationGroupId, 0);

	courses.reserve(N_COURSES);
	backgrounds.reserve(N_COURSES);

	for(uint i = 0; i < N_COURSES; ++i)
	{
		courses.push_back(moduleTexture->get(coursesBackgroundsTextureGroupId, 4 * (i + 1) - 1));
		backgrounds.push_back(moduleTexture->get(coursesBackgroundsTextureGroupId, 4 * i));
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
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAudio());
	assert(getGameEngine()->getModuleTexture());
	assert(getGameEngine()->getModuleAnimation());

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
	assert((int)backgrounds.size() > currentCourseId);

	const Texture* backgroundT = backgrounds[currentCourseId];

	assert(backgroundT);
	assert(backgroundT->r);

	backgroundOffsetX = mod0L(backgroundOffsetX + 40.0f * deltaTimeS, (float)backgroundT->r->w);
}

void ModuleCourseSelect::updateCourseChangeAnimation(float deltaTimeS)
{
	if(showChangeCourseAnimation)
	{
		assert(changeCourseAnimation);

		if(changeCourseAnimation->hasEnded()) showChangeCourseAnimation = false;
		else changeCourseAnimation->update(deltaTimeS);
	}
}

void ModuleCourseSelect::checkGoMenu() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());
	assert(getGameEngine()->getModuleSwitch());

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::START);
}

void ModuleCourseSelect::checkSelectOption() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());
	assert(getGameEngine()->getModuleSwitch());
	assert(getGameEngine()->getModuleRegistry());

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		getGameEngine()->getModuleRegistry()->setCurrentCourseId(currentCourseId);
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::FREE_PRACTICE);
	}
}

void ModuleCourseSelect::checkChangeCourse()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());

	int tmpCurrentCourseId = currentCourseId;

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RIGHT) == KeyState::DOWN)
		currentCourseId = mod0L(currentCourseId + 1, N_COURSES);
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_LEFT) == KeyState::DOWN)
		currentCourseId = mod0L(currentCourseId - 1, N_COURSES);

	if(currentCourseId != tmpCurrentCourseId)
	{
		assert(changeCourseAnimation);
		assert(getGameEngine()->getModuleAudio());
		assert(getGameEngine()->getModuleRegistry());

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
	assert(base);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	getGameEngine()->getModuleRenderer()->renderTexture(base->t, base->r, &baseRect, base->hFlipped);
}

void ModuleCourseSelect::renderCourseInfo() const
{
	assert(getGameEngine());

	ModuleFont* moduleFont = getGameEngine()->getModuleFont();

	assert(moduleFont);

	// Name

	moduleFont->renderText(COURSE_NAMES[currentCourseId], courseNamePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_NAME_POSITION_SCALE, COURSE_NAME_POSITION_SCALE, 248, 216, 0);

	// Round

	moduleFont->renderText("< ROUND >", courseRoundPosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 252, 252);
	moduleFont->renderText(to_string(currentCourseId + 1), courseRoundValuePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 216, 0);

	// Length

	moduleFont->renderText("LENGTH", courseLengthPosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 252, 252);
	
	string courseLengthStr = to_string((int)COURSE_LENGHTS[currentCourseId]); courseLengthStr += "M";
	moduleFont->renderText(courseLengthStr, courseLengthValuePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 216, 0);

	// Best lap time

	moduleFont->renderText("BEST LAP", courseBestLapTimePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 252, 252);
	moduleFont->renderText(currentBestLapTimeStr, courseBestLapTimeValuePosition, HAlignment::LEFT, VAlignment::BOTTOM, COURSE_INFO_VALUE_POSITION_SCALE_W, COURSE_INFO_VALUE_POSITION_SCALE_H, 248, 216, 0);
}

void ModuleCourseSelect::renderCourseBackground() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());
	assert((int)courses.size() > currentCourseId);
	assert((int)backgrounds.size() > currentCourseId);

	// Course

	const Texture* course = courses[currentCourseId];

	assert(course);

	getGameEngine()->getModuleRenderer()->renderTexture(course->t, course->r, &courseRect, course->hFlipped);

	// Background

	const Texture* background = backgrounds[currentCourseId];
	
	assert(background);
	
	SDL_Rect textureRect{ background->r->x, background->r->y, COURSE_BACKGROUND_REGION_X, COURSE_BACKGROUND_REGION_Y };

	SDL_Rect rect0, rect1, renderRect0, renderRect1;

	if(!getRectsEndlessTexture(background, textureRect, backgroundRect, false, backgroundOffsetX, rect0, renderRect0, rect1, renderRect1))
		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect0, &renderRect0, background->hFlipped);
	else
	{
		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect0, &renderRect0, background->hFlipped);
		getGameEngine()->getModuleRenderer()->renderTexture(background->t, &rect1, &renderRect1, background->hFlipped);
	}
}

void ModuleCourseSelect::renderChangeCourseAnimations() const
{
	if(showChangeCourseAnimation)
	{
		assert(getGameEngine());

		ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

		assert(moduleRenderer);
		assert(changeCourseAnimation);

		const Texture* animationT = changeCourseAnimation->getCurrentFrame();

		assert(animationT);

		for(SDL_Rect rect : changeCourseAnimationRects)
			moduleRenderer->renderTexture(animationT->t, animationT->r, &rect, animationT->hFlipped);
	}
}