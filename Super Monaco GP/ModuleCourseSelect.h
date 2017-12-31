#ifndef _MODULE_COURSE_SELECT_
#define _MODULE_COURSE_SELECT_

#include <vector>
#include "Types.h"
#include "Module.h"
#include <SDL_rect.h>
#include "ModuleCourseSelectUI.h"

class Animation;

class ModuleCourseSelect : public Module
{
	public:

		ModuleCourseSelect(GameEngine* gameEngine);

		virtual ~ModuleCourseSelect();

		virtual bool setUp() override;

		virtual bool update(float deltaTimeS) override;

		virtual void cleanUp() override;

	private:

		void addChangeAnimationRects(const SDL_Rect& rect, uint r, uint c);

		void updateBackground(float deltaTimeS);

		void updateCourseChangeAnimation(float deltaTimeS);

		void checkGoMenu() const;

		void checkSelectOption() const;

		void checkChangeCourse();

		void render() const;

		void renderBase() const;

		void renderCourseInfo() const;

		void renderCourseBackground() const;

		void renderChangeCourseAnimations() const;

	private:

		int currentCourseId = 0;

		uint audioGroupId = -1;

		uint courseSelectAnimationGroupId = -1;

		uint coursesBackgroundsTextureGroupId = -1;

		const Texture* base = nullptr;

		std::vector<const Texture*> courses;

		std::vector<const Texture*> backgrounds;

		bool showChangeCourseAnimation = true;

		Animation* changeCourseAnimation = nullptr;

		SDL_Rect baseRect, courseRect, backgroundRect;

		float backgroundOffsetX = 0.0f;

		std::vector<SDL_Rect> changeCourseAnimationRects;

		WindowPosition courseNamePosition;

		WindowPosition courseRoundPosition, courseRoundValuePosition;
		
		WindowPosition courseLengthPosition, courseLengthValuePosition;
		
		WindowPosition courseBestLapTimePosition, courseBestLapTimeValuePosition;

		std::string currentBestLapTimeStr;
};

#endif