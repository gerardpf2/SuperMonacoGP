#include "ModuleSega.h"

#include "Animation.h"
#include "GameEngine.h"
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
	getGameEngine()->getModuleAnimation()->unload(logoAnimationGroupId);

	logoAnimation = nullptr;
}

void ModuleSega::updateLogo(float deltaTimeS) const
{
	logoAnimation->update(deltaTimeS);
}

void ModuleSega::checkGoStart() const
{
	if(logoAnimation->hasEnded() || getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
		getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::START);
}

void ModuleSega::render() const
{
	getGameEngine()->getModuleRenderer()->renderRect(&baseAllRect, 0, 0, 0);

	const Texture* logoT = logoAnimation->getCurrentFrame();

	getGameEngine()->getModuleRenderer()->renderTexture(logoT->t, logoT->r, &logoRect, logoT->hFlipped);
}