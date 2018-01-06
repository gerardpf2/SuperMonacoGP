#include "ModuleSega.h"

#include <assert.h>
#include "Globals.h"
#include "Animation.h"
#include "ModuleInput.h"
#include "ModuleSegaUI.h"
#include "ModuleSwitch.h"
#include "ModuleRenderer.h"
#include "ModuleAnimation.h"

ModuleSega::ModuleSega(GameEngine* gameEngine) :
	Module(gameEngine)
{
	baseAllRect = BASE_ALL_RECT;

	logoRect = LOGO_RECT;
}

ModuleSega::~ModuleSega()
{ }

bool ModuleSega::setUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAnimation());

	logoAnimationGroupId = getGameEngine()->getModuleAnimation()->load("Resources/Configurations/Animations/Sega.json");

	logoAnimation = getGameEngine()->getModuleAnimation()->getAnimation(logoAnimationGroupId, 0);

	return true;
}

bool ModuleSega::update(float deltaTimeS)
{
	if(!getBlocked())
	{
		checkGoStart();

		updateLogo(deltaTimeS);
	}

	render();

	return true;
}

void ModuleSega::cleanUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAnimation());

	getGameEngine()->getModuleAnimation()->unload(logoAnimationGroupId);

	logoAnimation = nullptr;
}

void ModuleSega::updateLogo(float deltaTimeS) const
{
	assert(logoAnimation);

	logoAnimation->update(deltaTimeS);
}

void ModuleSega::checkGoStart() const
{
	assert(logoAnimation);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());
	assert(getGameEngine()->getModuleSwitch());

	if(logoAnimation->hasEnded() || getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::START);
}

void ModuleSega::render() const
{
	assert(logoAnimation);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	getGameEngine()->getModuleRenderer()->renderRect(&baseAllRect, 0, 0, 0);

	const Texture* logoT = logoAnimation->getCurrentFrame();

	assert(logoT);

	getGameEngine()->getModuleRenderer()->renderTexture(logoT->t, logoT->r, &logoRect, logoT->hFlipped);
}