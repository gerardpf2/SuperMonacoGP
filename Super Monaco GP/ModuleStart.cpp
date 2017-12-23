#include "ModuleStart.h"

#include "Utils.h"
#include "GameEngine.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleWorld.h"
#include "ModuleTexture.h"
#include "ModuleRenderer.h"

ModuleStart::ModuleStart(GameEngine* gameEngine) :
	Module(gameEngine)
{
	backRect = BACK_RECT;
	frontRect = FRONT_RECT;
	logoRect = LOGO_RECT;
	optionsRect = OPTIONS_RECT;

	pressEnterPosition = PRESS_ENTER_POSITION;

	option0Position = OPTION_0_POSITION;
	option1Position = OPTION_1_POSITION;
	option2Position = OPTION_2_POSITION;

	arrowRects[0] = ARROW_0_RECT;
	arrowRects[1] = ARROW_1_RECT;
	arrowRects[2] = ARROW_2_RECT;
}

ModuleStart::~ModuleStart()
{ }

bool ModuleStart::setUp()
{
	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	textureGroupId = moduleTexture->load("Resources/Configurations/Textures/Start.json");

	back = moduleTexture->get(textureGroupId, 0);
	front = moduleTexture->get(textureGroupId, 1);
	logo = moduleTexture->get(textureGroupId, 2);
	options = moduleTexture->get(textureGroupId, 3);
	arrow = moduleTexture->get(textureGroupId, 4);

	return true;
}

bool ModuleStart::update(float deltaTimeS)
{
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN) return false;

	updateCars(deltaTimeS);
	renderBase();

	if(!enterPressed)
	{
		updateEnterNoPressed(deltaTimeS);
		renderEnterNoPressed();
	}
	else
	{
		updateEnterPressed(deltaTimeS);
		renderEnterPressed();
	}

	return true;
}

void ModuleStart::cleanUp()
{
	getGameEngine()->getModuleTexture()->unload(textureGroupId);

	back = nullptr;
	front = nullptr;
	logo = nullptr;
	options = nullptr;
}

void ModuleStart::updateCars(float deltaTimeS)
{

}

void ModuleStart::updateEnterPressed(float deltaTimeS)
{
	// Update arrow
	
	arrowCounter = mod0L(arrowCounter + 5.0f * deltaTimeS, 1.0f);

	// Update selected option

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_UP) == KeyState::DOWN)
		selectedOption = mod0L(selectedOption - 1, 3);
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_DOWN) == KeyState::DOWN)
		selectedOption = mod0L(selectedOption + 1, 3);

	// Selection has been done

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		switch(selectedOption)
		{
			case 0: // SUPER MONACO GP
				break;
			case 1: // FREE PRACTICE
				getGameEngine()->setGameModule(new ModuleWorld(getGameEngine()));

				break;
			case 2: // ABOUT
				break;
		}
	}
}

void ModuleStart::updateEnterNoPressed(float deltaTimeS)
{
	// Update press enter

	pressEnterCounter = mod0L(pressEnterCounter + deltaTimeS, 1.0f);

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
		enterPressed = true;
}

void ModuleStart::selectOption() const
{

}

void ModuleStart::renderBase() const
{
	// Render back, cars and front

	getGameEngine()->getModuleRenderer()->renderTexture(back->t, back->r, &backRect, back->hFlipped);

	renderCars();

	getGameEngine()->getModuleRenderer()->renderTexture(front->t, front->r, &frontRect, front->hFlipped);
}

void ModuleStart::renderCars() const
{

}

void ModuleStart::renderEnterPressed() const
{
	// Render optionsBox, options and arrow

	getGameEngine()->getModuleRenderer()->renderTexture(options->t, options->r, &optionsRect, options->hFlipped);

	getGameEngine()->getModuleFont()->renderText("SUPER MONACO GP", option0Position, HAlignment::LEFT, VAlignment::CENTER, OPTION_POSITION_SCALE_W, OPTION_POSITION_SCALE_H, 248, 252, 248);
	getGameEngine()->getModuleFont()->renderText("FREE PRACTICE", option1Position, HAlignment::LEFT, VAlignment::CENTER, OPTION_POSITION_SCALE_W, OPTION_POSITION_SCALE_H, 248, 252, 248);
	getGameEngine()->getModuleFont()->renderText("ABOUT", option2Position, HAlignment::LEFT, VAlignment::CENTER, OPTION_POSITION_SCALE_W, OPTION_POSITION_SCALE_H, 248, 252, 248);

	if(arrowCounter <= 0.5f)
		getGameEngine()->getModuleRenderer()->renderTexture(arrow->t, arrow->r, &arrowRects[selectedOption], arrow->hFlipped);
}

void ModuleStart::renderEnterNoPressed() const
{
	// Render logo and press enter

	getGameEngine()->getModuleRenderer()->renderTexture(logo->t, logo->r, &logoRect, logo->hFlipped);

	if(pressEnterCounter <= 0.5f)
		getGameEngine()->getModuleFont()->renderText("PRESS ENTER", pressEnterPosition, HAlignment::CENTER, VAlignment::CENTER, PRESS_ENTER_POSITION_SCALE, PRESS_ENTER_POSITION_SCALE, 248, 252, 248);
}