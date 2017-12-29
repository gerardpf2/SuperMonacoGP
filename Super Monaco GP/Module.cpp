#include "Module.h"

Module::Module(GameEngine* gameEngine) :
	gameEngine(gameEngine)
{ }

Module::~Module()
{ }

GameEngine* Module::getGameEngine() const
{
	return gameEngine;
}

bool Module::getBlocked() const
{
	return blocked;
}

void Module::setBlocked(bool blocked)
{
	this->blocked = blocked;
}

bool Module::setUp()
{
	return true;
}

bool Module::preUpdate(float deltaTimeS)
{
	return true;
}

bool Module::update(float deltaTimeS)
{
	return true;
}

bool Module::postUpdate(float deltaTimeS)
{
	return true;
}

void Module::cleanUp()
{ }