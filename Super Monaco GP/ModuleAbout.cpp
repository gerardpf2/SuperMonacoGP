#include "ModuleAbout.h"

#include <assert.h>
#include "Globals.h"
#include "Animation.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleSwitch.h"
#include "ModuleAboutUI.h"
#include "ModuleRenderer.h"
#include "ModuleAnimation.h"

ModuleAbout::ModuleAbout(GameEngine* gameEngine) :
	Module(gameEngine)
{
	baseAllRect = BASE_ALL_RECT;

	carRect = CAR_RECT;

	aboutPosition = ABOUT_POSITION;

	projectPosition = PROJECT_POSITION;
	projectValuePosition = PROJECT_VALUE_POSITION;

	techPosition = TECH_POSITION;
	techValuePosition = TECH_VALUE_POSITION;

	authorPosition = AUTHOR_POSITION;
	authorValuePosition = AUTHOR_VALUE_POSITION;

	separationPosition = SEPARATION_POSITION;

	controlsPosition = CONTROLS_POSITION;
	controlsValuePosition0 = CONTROLS_VALUE_POSITION_0;
	controlsValuePosition1 = CONTROLS_VALUE_POSITION_1;
	controlsValuePosition2 = CONTROLS_VALUE_POSITION_2;
	controlsValuePosition3 = CONTROLS_VALUE_POSITION_3;
}

ModuleAbout::~ModuleAbout()
{ }

bool ModuleAbout::setUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAnimation());

	carAnimationGroupId = getGameEngine()->getModuleAnimation()->load("Resources/Configurations/Animations/About.json");

	carAnimation = getGameEngine()->getModuleAnimation()->getAnimation(carAnimationGroupId, 0);

	return true;
}

bool ModuleAbout::update(float deltaTimeS)
{
	if(!getBlocked())
	{
		checkGoStart();

		updateCar(deltaTimeS);
	}

	render();

	return true;
}

void ModuleAbout::cleanUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAnimation());

	getGameEngine()->getModuleAnimation()->unload(carAnimationGroupId);

	carAnimation = nullptr;
}

void ModuleAbout::checkGoStart() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());
	assert(getGameEngine()->getModuleSwitch());

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN || getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN)
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::START);
}

void ModuleAbout::updateCar(float deltaTimeS)
{
	assert(carAnimation);

	carAnimation->update(deltaTimeS);
}

void ModuleAbout::render() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	getGameEngine()->getModuleRenderer()->renderRect(&baseAllRect, 0, 0, 0);

	renderCar();

	renderInfo();
}

void ModuleAbout::renderCar() const
{
	assert(carAnimation);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	const Texture* carT = carAnimation->getCurrentFrame();

	assert(carT);

	getGameEngine()->getModuleRenderer()->renderTexture(carT->t, carT->r, &carRect, carT->hFlipped);
}

void ModuleAbout::renderInfo() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleFont());

	getGameEngine()->getModuleFont()->renderText("ABOUT", aboutPosition, HAlignment::CENTER, VAlignment::BOTTOM, ABOUT_POSITION_SCALE, ABOUT_POSITION_SCALE, 224, 160, 0);

	getGameEngine()->getModuleFont()->renderText("PROJECT", projectPosition, HAlignment::LEFT, VAlignment::CENTER, POSITION_SCALE, POSITION_SCALE, 224, 160, 0);
	getGameEngine()->getModuleFont()->renderText("SUPER MONACO GP CLONE", projectValuePosition, HAlignment::LEFT, VAlignment::CENTER, VALUE_POSITION_SCALE, VALUE_POSITION_SCALE, 248, 252, 248);

	getGameEngine()->getModuleFont()->renderText("TECH", techPosition, HAlignment::LEFT, VAlignment::CENTER, POSITION_SCALE, POSITION_SCALE, 224, 160, 0);
	getGameEngine()->getModuleFont()->renderText("C++ AND SDL", techValuePosition, HAlignment::LEFT, VAlignment::CENTER, VALUE_POSITION_SCALE, VALUE_POSITION_SCALE, 248, 252, 248);

	getGameEngine()->getModuleFont()->renderText("AUTHOR", authorPosition, HAlignment::LEFT, VAlignment::CENTER, POSITION_SCALE, POSITION_SCALE, 224, 160, 0);
	getGameEngine()->getModuleFont()->renderText("GERARD PF", authorValuePosition, HAlignment::LEFT, VAlignment::CENTER, VALUE_POSITION_SCALE, VALUE_POSITION_SCALE, 248, 252, 248);

	getGameEngine()->getModuleFont()->renderText("------------------------------", separationPosition, HAlignment::CENTER, VAlignment::CENTER, POSITION_SCALE, POSITION_SCALE, 224, 160, 0);

	getGameEngine()->getModuleFont()->renderText("CONTROLS", controlsPosition, HAlignment::LEFT, VAlignment::CENTER, POSITION_SCALE, POSITION_SCALE, 224, 160, 0);
	getGameEngine()->getModuleFont()->renderText("WASD - DRIVE", controlsValuePosition0, HAlignment::LEFT, VAlignment::CENTER, VALUE_POSITION_SCALE, VALUE_POSITION_SCALE, 248, 252, 248);
	getGameEngine()->getModuleFont()->renderText("ESCAPE - GO BACK", controlsValuePosition1, HAlignment::LEFT, VAlignment::CENTER, VALUE_POSITION_SCALE, VALUE_POSITION_SCALE, 248, 252, 248);
	getGameEngine()->getModuleFont()->renderText("ENTER - SELECT OPTION", controlsValuePosition2, HAlignment::LEFT, VAlignment::CENTER, VALUE_POSITION_SCALE, VALUE_POSITION_SCALE, 248, 252, 248);
	getGameEngine()->getModuleFont()->renderText("ARROWS - DRIVE AND CHANGE OPTION", controlsValuePosition3, HAlignment::LEFT, VAlignment::CENTER, VALUE_POSITION_SCALE, VALUE_POSITION_SCALE, 248, 252, 248);
}