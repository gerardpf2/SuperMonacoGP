#include "Module.h"

Module::Module(GameEngine* gameEngine, bool active) :
	gameEngine(gameEngine), active(active)
{ }

Module::~Module()
{ }

bool Module::setUp()
{
	return true;
}

bool Module::preUpdate()
{
	return true;
}

bool Module::update(float deltaTimeS)
{
	return true;
}

bool Module::postUpdate()
{
	return true;
}

void Module::cleanUp()
{ }

GameEngine* Module::getGameEngine() const
{
	return gameEngine;
}

bool Module::getActive() const
{
	return active;
}

void Module::setActive(bool active)
{
	this->active = active;
}