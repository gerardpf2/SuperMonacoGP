#include "ModuleStart.h"

#include "Utils.h"
#include "ModuleFont.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSwitch.h"
#include "ModuleTexture.h"
#include "ModuleStartUI.h"
#include "ModuleRenderer.h"
#include "ModuleRegistry.h"

using namespace std;

struct CarStart
{
	// Simple structure that helps managing the cars that appear in this menu

	int y;
	float x, v;
	const Texture* t;

	CarStart(const Texture* t) :
		t(t)
	{ }

	bool isOut()
	{
		return x + CAR_START_W <= CAR_MIN_X;
	}

	void update(float deltaTimeS)
	{
		x -= v * deltaTimeS;
	}
};

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
	assert(getGameEngine());

	ModuleTexture* moduleTexture = getGameEngine()->getModuleTexture();

	assert(moduleTexture);

	textureGroupId = moduleTexture->load("Resources/Configurations/Textures/Start.json");

	back = moduleTexture->get(textureGroupId, 0);
	front = moduleTexture->get(textureGroupId, 1);
	logo = moduleTexture->get(textureGroupId, 2);
	options = moduleTexture->get(textureGroupId, 3);
	arrow = moduleTexture->get(textureGroupId, 4);

	cars.reserve(6);

	cars.push_back(new CarStart(moduleTexture->get(textureGroupId, 5)));
	cars.push_back(new CarStart(moduleTexture->get(textureGroupId, 6)));
	cars.push_back(new CarStart(moduleTexture->get(textureGroupId, 7)));
	cars.push_back(new CarStart(moduleTexture->get(textureGroupId, 8)));
	cars.push_back(new CarStart(moduleTexture->get(textureGroupId, 9)));
	cars.push_back(new CarStart(moduleTexture->get(textureGroupId, 10)));

	carIndex = rand() % cars.size();

	assert(getGameEngine()->getModuleAudio());
	assert(getGameEngine()->getModuleRegistry());

	getGameEngine()->getModuleRegistry()->setCurrentCourseId(0);

	audioGroupId = getGameEngine()->getModuleAudio()->load("Resources/Configurations/Audios/Start.json");

	getGameEngine()->getModuleAudio()->playMusic(audioGroupId, 0, 1.0f, 0);

	return true;
}

bool ModuleStart::update(float deltaTimeS)
{
	if(!getBlocked())
	{
		assert(getGameEngine());
		assert(getGameEngine()->getModuleInput());

		if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_ESCAPE) == KeyState::DOWN) return false;

		updateCars(deltaTimeS);

		if(!enterPressed) updateEnterNoPressed(deltaTimeS);
		else updateEnterPressed(deltaTimeS);
	}

	render();

	return true;
}

void ModuleStart::cleanUp()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleAudio());
	assert(getGameEngine()->getModuleTexture());

	getGameEngine()->getModuleAudio()->unload(audioGroupId);

	getGameEngine()->getModuleTexture()->unload(textureGroupId);

	back = nullptr;
	front = nullptr;
	logo = nullptr;
	options = nullptr;

	for(CarStart* car : cars)
	{
		delete car; car = nullptr;
	}

	cars.clear();

	usedCars.clear();
}

void ModuleStart::updateCars(float deltaTimeS)
{
	uint n = 1;

	if(usedCars.empty()) ++usedCars2Freq;

	if(usedCars2Freq >= USED_CARS_2_FREQ)
	{
		n = 2;
		usedCars2Freq = 0;
	}

	while(usedCars.size() < n)
	{
		assert(!cars.empty());

		CarStart* car = cars[carIndex++ % cars.size()];

		assert(car);

		list<CarStart*>::iterator it = usedCars.begin();

		car->x = CAR_START_MAX_X;

		if(usedCars.empty())
		{
			car->y = (rand() % 2 == 0) ? CAR_START_MIN_Y : CAR_START_MAX_Y;
			car->v = CAR_START_MAX_V;
		}
		else
		{
			assert(*it);

			car->y = ((*it)->y == CAR_START_MIN_Y) ? CAR_START_MAX_Y : CAR_START_MIN_Y;
			car->v = CAR_START_MIN_V;
		}

		while(it != usedCars.end() && *it && (*it)->y <= car->y) ++it;

		usedCars.insert(it, car);
	}

	for(list<CarStart*>::iterator it = usedCars.begin(); it != usedCars.end(); )
	{
		CarStart* car = *it;

		assert(car);

		car->update(deltaTimeS);

		if(car->isOut()) it = usedCars.erase(it);

		if(it != usedCars.end()) ++it;
	}
}

void ModuleStart::updateEnterPressed(float deltaTimeS)
{
	// Update arrow
	
	arrowCounter = mod0L(arrowCounter + 5.0f * deltaTimeS, 1.0f);

	checkChangeOption();

	checkSelectOption();
}

void ModuleStart::updateEnterNoPressed(float deltaTimeS)
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());

	// Update press enter

	pressEnterCounter = mod0L(pressEnterCounter + deltaTimeS, 1.0f);

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		assert(getGameEngine()->getModuleAudio());

		enterPressed = true;

		getGameEngine()->getModuleAudio()->playMusic(audioGroupId, 1);
	}
}

void ModuleStart::checkSelectOption() const
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());

	// Selection has been done

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_RETURN) == KeyState::DOWN)
	{
		assert(getGameEngine()->getModuleSwitch());

		switch(selectedOption)
		{
			case 0: // SUPER MONACO GP
				getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::SUPER_MONACO_GP);

				break;
			case 1: // COURSE_SELECT, FREE PRACTICE
				getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::COURSE_SELECT);

				break;
			case 2: // ABOUT
				getGameEngine()->getModuleSwitch()->setNewGameModule(GameModule::ABOUT);

				break;
		}
	}
}

void ModuleStart::checkChangeOption()
{
	assert(getGameEngine());
	assert(getGameEngine()->getModuleInput());
	assert(getGameEngine()->getModuleAudio());

	// Update selected option

	int tmpSelectedOption = selectedOption;

	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_UP) == KeyState::DOWN) selectedOption = mod0L(selectedOption - 1, 3);
	if(getGameEngine()->getModuleInput()->getKeyState(SDL_SCANCODE_DOWN) == KeyState::DOWN) selectedOption = mod0L(selectedOption + 1, 3);

	if(selectedOption != tmpSelectedOption)
		getGameEngine()->getModuleAudio()->playFx(audioGroupId, 0);
}

void ModuleStart::render() const
{
	renderBase();

	if(!enterPressed) renderEnterNoPressed();
	else renderEnterPressed();
}

void ModuleStart::renderBase() const
{
	assert(back);
	assert(front);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleRenderer());

	// Render back, cars and front

	getGameEngine()->getModuleRenderer()->renderTexture(back->t, back->r, &backRect, back->hFlipped);

	renderCars();

	getGameEngine()->getModuleRenderer()->renderTexture(front->t, front->r, &frontRect, front->hFlipped);
}

void ModuleStart::renderCars() const
{
	assert(getGameEngine());

	ModuleRenderer* moduleRenderer = getGameEngine()->getModuleRenderer();

	assert(moduleRenderer);

	for(CarStart* car : usedCars)
	{
		assert(car);
		assert(car->t);

		SDL_Rect rect{ (int)car->x, car->y, CAR_START_W, CAR_START_H };
		moduleRenderer->renderTexture(car->t->t, car->t->r, &rect, car->t->hFlipped);
	}
}

void ModuleStart::renderEnterPressed() const
{
	assert(arrow);
	assert(options);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleFont());
	assert(getGameEngine()->getModuleRenderer());

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
	assert(logo);
	assert(getGameEngine());
	assert(getGameEngine()->getModuleFont());
	assert(getGameEngine()->getModuleRenderer());

	// Render logo and press enter

	getGameEngine()->getModuleRenderer()->renderTexture(logo->t, logo->r, &logoRect, logo->hFlipped);

	if(pressEnterCounter <= 0.5f)
		getGameEngine()->getModuleFont()->renderText("PRESS ENTER", pressEnterPosition, HAlignment::CENTER, VAlignment::CENTER, PRESS_ENTER_POSITION_SCALE, PRESS_ENTER_POSITION_SCALE, 248, 252, 248);
}